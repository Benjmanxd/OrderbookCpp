#pragma once

#include <vector>

#include "types.h"

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
