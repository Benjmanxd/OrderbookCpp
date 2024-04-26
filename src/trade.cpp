#include "trade.h"

Trade::Trade(const TradeInfo &ask_trade, const TradeInfo &bid_trade)
    : m_ask_trade(ask_trade), m_bid_trade(bid_trade) {}
