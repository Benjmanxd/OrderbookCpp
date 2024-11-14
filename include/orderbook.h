#pragma once

#include <atomic>
#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "levelinfo.h"
#include "order.h"
#include "trade.h"

class Orderbook {
public:
  Orderbook();
  ~Orderbook();
  Trades AddOrder(const OrderPtr &);
  void CancelOrder(OrderId);
  void CancelOrders(const OrderIds &);
  template <typename order_class> Trades ModifyOrder(OrderModify<order_class>);
  // OrderbookLevelInfos GetLevelInfos() const;
  LevelInfoss GetLevelInfos() const;

  std::size_t Size() const { return m_orders.size(); }
  void Print() const;

private:
  struct OrderEntry {
    OrderPtr m_order{nullptr};
    OrderPtrs::iterator m_pos;
  };
  void CancelOrderInternal(OrderId);
  bool MatchPrice(Side, Price) const;
  bool MatchQuantity(Side, Price, Quantity) const;
  Trades MatchOrders();
  void PruneDayOrders();

  std::atomic<bool> m_closed{false};
  std::condition_variable m_closed_cv;
  std::mutex mutable m_order_mutex;
  std::thread m_prune_thread;

  std::map<Price, OrderPtrs, std::less<Price>> m_asks;
  std::map<Price, OrderPtrs, std::greater<Price>> m_bids;
  std::unordered_map<OrderId, OrderEntry> m_orders;
};
