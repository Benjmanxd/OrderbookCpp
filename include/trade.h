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
  Trade(const TradeInfo &, const TradeInfo &);

  const TradeInfo &GetAskTrade() const { return m_ask_trade; }
  const TradeInfo &GetBidTrade() const { return m_bid_trade; }

private:
  TradeInfo m_ask_trade;
  TradeInfo m_bid_trade;
};

using Trades = std::vector<Trade>;
