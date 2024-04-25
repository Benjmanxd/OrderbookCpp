#pragma once

#include <vector>

#include "types.h"

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
