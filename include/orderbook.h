#pragma once

#include <map>
#include <unordered_map>

#include "levelinfo.h"
#include "order.h"
#include "trade.h"

class Orderbook {
public:
  Trades AddOrder(const OrderPtr &);
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
