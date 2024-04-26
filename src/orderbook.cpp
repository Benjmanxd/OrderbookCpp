#include "orderbook.h"

#include <chrono>
#include <ctime>
#include <numeric>

Orderbook::Orderbook() : m_prune_thread{[this] { PruneDayOrders(); }} {}

Orderbook::~Orderbook() {
  m_closed.store(true, std::memory_order_release);
  m_closed_cv.notify_one();
  m_prune_thread.join();
}

Trades Orderbook::AddOrder(const OrderPtr &order) {
  if (m_orders.count(order->GetOrderId()))
    return {};

  if (order->GetOrderType() == OrderType::Market) {
    if (order->GetSide() == Side::Buy && !m_asks.empty()) {
      const auto &[price, _] = *m_asks.rbegin();
      order->PriceAdjust(price);
    } else if (order->GetSide() == Side::Sell && !m_bids.empty()) {
      const auto &[price, _] = *m_bids.rbegin();
      order->PriceAdjust(price);
    } else {
      return {};
    }
  }

  if (order->GetOrderType() == OrderType::FillAndKill && !Match(order->GetSide(), order->GetPrice()))
    return {};

  OrderPtrs::iterator iter;
  if (order->GetSide() == Side::Buy) {
    auto &orders = m_bids[order->GetPrice()];
    orders.push_back(order);
    iter = std::prev(orders.end());
  } else {
    auto &orders = m_asks[order->GetPrice()];
    orders.push_back(order);
    iter = std::prev(orders.end());
  }
  m_orders[order->GetOrderId()] = {order, iter};
  return MatchOrders();
}

void Orderbook::CancelOrder(OrderId order_id) {
  std::scoped_lock l(m_order_mutex);
  CancelOrderInternal(order_id);
}

void Orderbook::CancelOrders(const OrderIds& order_ids) {
  std::scoped_lock l(m_order_mutex);
  for (OrderId id : order_ids) {
    CancelOrder(id);
  }
}

Trades Orderbook::MatchOrder(OrderModify order) {
  auto iter = m_orders.find(order.GetOrderId());
  if (iter == m_orders.end())
    return {};

  const auto &[order_ptr, _] = iter->second;
  CancelOrder(order_ptr->GetOrderId());
  return AddOrder(order.Convert(order_ptr->GetOrderType()));
}

void Orderbook::CancelOrderInternal(OrderId order_id) {
  auto iter = m_orders.find(order_id);
  if (iter == m_orders.end())
    return;

  const auto &[order_ptr, order_iter] = iter->second;
  auto price = order_ptr->GetPrice();
  if (order_ptr->GetSide() == Side::Buy) {
    auto &orders = m_bids[price];
    orders.erase(order_iter);

    if (orders.empty())
      m_bids.erase(price);
  } else {
    auto &orders = m_asks[price];
    orders.erase(order_iter);

    if (orders.empty())
      m_asks.erase(price);
  }
  m_orders.erase(iter);
}

OrderbookLevelInfos Orderbook::GetLevelInfos() const {
  LevelInfos ask_infos, bid_infos;

  auto CreateLevelInfo = [](Price price, OrderPtrs orders) -> LevelInfo {
    return {price, std::accumulate(orders.begin(), orders.end(), (Quantity)0, [](Quantity sum, OrderPtr order_ptr) {
              return sum + order_ptr->GetRemainingQuantity();
            })};
  };

  for (const auto &[ask_price, ask_orders] : m_asks)
    ask_infos.emplace_back(CreateLevelInfo(ask_price, ask_orders));

  for (const auto &[bid_price, bid_orders] : m_bids)
    bid_infos.emplace_back(CreateLevelInfo(bid_price, bid_orders));

  return OrderbookLevelInfos{ask_infos, bid_infos};
}

bool Orderbook::Match(Side side, Price price) const {
  return side == Side::Buy ? !m_asks.empty() && m_asks.begin()->first <= price
                           : !m_bids.empty() && m_bids.begin()->first >= price;
}

Trades Orderbook::MatchOrders() {
  Trades trades;
  trades.reserve(m_orders.size());

  while (true) {
    if (m_asks.empty() || m_bids.empty())
      break;

    auto &[ask_price, ask_orders] = *m_asks.begin();
    auto &[bid_price, bid_orders] = *m_bids.begin();
    if (ask_price > bid_price) {
      break;
    }

    while (ask_orders.size() && bid_orders.size()) {
      auto &ask = ask_orders.front();
      auto &bid = bid_orders.front();

      Quantity quantity = std::min(ask->GetRemainingQuantity(), bid->GetRemainingQuantity());
      ask->Fill(quantity);
      bid->Fill(quantity);

      if (ask->IsFilled()) {
        ask_orders.pop_front();
        m_orders.erase(ask->GetOrderId());
      }
      if (bid->IsFilled()) {
        bid_orders.pop_front();
        m_orders.erase(bid->GetOrderId());
      }

      if (ask_orders.empty())
        m_asks.erase(ask_price);
      if (bid_orders.empty())
        m_bids.erase(bid_price);

      trades.emplace_back(
          TradeInfo{ask->GetOrderId(), ask->GetPrice(), quantity},
          TradeInfo{bid->GetOrderId(), bid->GetPrice(), quantity});
    }
  }

  if (!m_asks.empty()) {
    auto &[_, ask_orders] = *m_asks.begin();
    auto &order = ask_orders.front();
    if (order->GetOrderType() == OrderType::FillAndKill) {
      CancelOrder(order->GetOrderId());
    }
  }

  if (!m_bids.empty()) {
    auto &[_, bid_orders] = *m_bids.begin();
    auto &order = bid_orders.front();
    if (order->GetOrderType() == OrderType::FillAndKill) {
      CancelOrder(order->GetOrderId());
    }
  }

  return trades;
}

void Orderbook::PruneDayOrders() {
  while (true) {
    const auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm *now = std::localtime(&tt);
    std::tm end {0, 0, 16, now->tm_hour < 16 ? now->tm_mday : now->tm_mday+1, now->tm_mon, now->tm_year};

    {
      std::unique_lock<std::mutex> l(m_order_mutex);
      if (m_closed.load(std::memory_order_acquire) || m_closed_cv.wait_until(l, std::chrono::system_clock::from_time_t(std::mktime(&end))) == std::cv_status::no_timeout)
        return;
    }

    OrderIds ids;
    {
      std::lock_guard<std::mutex> l(m_order_mutex);
      for (const auto& [_, order_entry] : m_orders) {
        if (static_cast<int>(order_entry.m_order->GetOrderType()) >= 10) {
          ids.push_back(order_entry.m_order->GetOrderId());
        }
      }
    }
    CancelOrders(ids);
  }
}
