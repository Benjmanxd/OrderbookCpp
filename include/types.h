#pragma once

#include <cstdint>

using Price = int32_t;
using Quantity = uint32_t;
using OrderId = uint64_t;

enum class OrderType : uint8_t {
  GoodTillCancel,
  FillAndKill,
  Market,
  GoodForDay = 10,
};

enum class Side : uint8_t {
  Buy = 0,
  Sell = 1,
};
