#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace OrderbookCore {
using Price = int32_t;
using Quantity = uint32_t;
using OrderId = uint64_t;

enum class OrderType : uint8_t {
  GoodTillCancel,
  FillAndKill,
  FillOrKill,
  Market,
  GoodForDay = 10,
  Unknown = 100,
};

inline const std::unordered_map<OrderType, std::string> OrderTypeMap = {
    {OrderType::GoodTillCancel, "Good Till Cancel"},
    {OrderType::FillAndKill, "Fill And Kill"},
    {OrderType::FillOrKill, "Fill Or Kill"},
    {OrderType::Market, "Market"},
    {OrderType::GoodForDay, "Good For Day"},
};

inline const char* OrderTypeItems[] = { "GTC", "FAK", "FOK", "M", "GFD" };

enum class Side : uint8_t {
  Buy = 0,
  Sell = 1,
  SideCount = 2,
  Unknown = 3,
};

inline const char* SideItems[] = { "Buy", "Sell" };
}
