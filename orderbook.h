#ifndef _ORDERBOOK_H_
#define _ORDERBOOK_H_

#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <cassert>

using Price = int32_t;
using Quantity = uint32_t;
using OrderId = uint64_t;

enum class OrderType : uint8_t {
  GoodTillCancel,
  FillAndKill,
};

enum class Side : uint8_t {
  Buy = 0,
  Sell = 1,
};

struct LevelInfo {
  Price m_price;
  Quantity m_quantity;
};

using LevelInfos = std::vector<LevelInfo>;

class OrderbookLevelInfos {
public:
  OrderbookLevelInfos(const LevelInfos &asks, const LevelInfos &bids)
      : m_asks(asks), m_bids(bids) {}

  const LevelInfos &GetAsks() const { return m_asks; }
  const LevelInfos &GetBids() const { return m_bids; }
  LevelInfos &GetAsks() { return m_asks; }
  LevelInfos &GetBids() { return m_bids; }

private:
  LevelInfos m_asks;
  LevelInfos m_bids;
};

class Order {
public:
  Order(OrderType order_type, OrderId order_id, Side side, Price price,
        Quantity quantity)
      : m_order_type(order_type), m_order_id(order_id), m_side(side),
        m_price(price), m_initial_quantity(quantity),
        m_remaining_quantity(quantity) {}

  OrderType GetOrderType() const { return m_order_type; }
  OrderId GetOrderId() const { return m_order_id; }
  Side GetSide() const { return m_side; }
  Price GetPrice() const { return m_price; }
  Quantity GetInitialQuantity() const { return m_initial_quantity; }
  Quantity GetRemainingQuantity() const { return m_remaining_quantity; }
  bool IsFilled() const { return m_remaining_quantity == 0; }
  bool Fill(Quantity quantity);

private:
  OrderType m_order_type;
  OrderId m_order_id;
  Side m_side;
  Price m_price;
  Quantity m_remaining_quantity;
  Quantity m_initial_quantity;
};

using OrderPtr = std::shared_ptr<Order>;
using OrderPtrs = std::list<OrderPtr>;

class OrderModify {
public:
  OrderModify(OrderId order_id, Side side, Price price, Quantity quantity)
      : m_order_id(order_id), m_side(side), m_price(price),
        m_quantity(quantity) {}

  OrderId GetOrderId() const { return m_order_id; }
  Side GetSide() const { return m_side; }
  Price GetPrice() const { return m_price; }
  Quantity GetQuantity() const { return m_quantity; }
  OrderPtr Convert(OrderType) const;

private:
  OrderId m_order_id;
  Side m_side;
  Price m_price;
  Quantity m_quantity;
};

struct TradeInfo {
  OrderId m_order_id;
  Price m_price;
  Quantity m_quantity;
};

class Trade {
public:
  Trade(const TradeInfo &ask_trade, const TradeInfo &bid_trade)
      : m_ask_trade(ask_trade), m_bid_trade(bid_trade) {}

  const TradeInfo &GetAskTrade() const { return m_ask_trade; }
  const TradeInfo &GetBidTrade() const { return m_bid_trade; }

private:
  TradeInfo m_ask_trade;
  TradeInfo m_bid_trade;
};

using Trades = std::vector<Trade>;

class Orderbook {
public:
  Trades AddOrder(const OrderPtr&);
  void CancelOrder(OrderId);
  Trades MatchOrder(OrderModify);
  OrderbookLevelInfos GetLevelInfos() const;
  std::size_t Size() const { return m_orders.size(); }

private:
  struct OrderEntry {
    OrderPtr m_order{nullptr};
    OrderPtrs::iterator m_pos;
  };
  bool Match(Side, Price) const;
  Trades MatchOrders();

  std::map<Price, OrderPtrs, std::less<Price>> m_asks;
  std::map<Price, OrderPtrs, std::greater<Price>> m_bids;
  std::unordered_map<OrderId, OrderEntry> m_orders;
};

#endif
