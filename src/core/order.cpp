#include "core/order.h"

#include <cstring>
#include <stdexcept>

#include "core/util.h"

namespace OrderbookCore {
template <typename order_class, Side side> void Order<order_class, side>::Fill(Quantity quantity) {
  if (quantity > m_remaining_quantity)
    throw std::logic_error("Order cannot be filled more than its remaining quantity");

  m_remaining_quantity -= quantity;
}

template <typename order_class, Side side> void Order<order_class, side>::PriceAdjust(Price price) {
  if (m_order_type != OrderType::Market)
    throw std::logic_error("Only market orders can adjust price");

  m_order_type = OrderType::GoodTillCancel;
  m_price = price;
}

template <typename order_class> OrderPtr OrderModify<order_class>::Convert() const {
  return std::make_shared<order_class>(m_order_id, m_price, m_quantity);
}

OrderPtr OrderFactory::CreateOrder(const char *side, const char *type, Quantity quantity, Price price) {
  static uint64_t order_id = 0;
  switch (hash_strlit(type)) {
  case "GTC"_hash:
    return !strcmp(side, "Buy") ? static_cast<OrderPtr>(std::make_shared<GoodTillCancelOrder<Side::Buy>>(++order_id, price, quantity))
                                : static_cast<OrderPtr>(std::make_shared<GoodTillCancelOrder<Side::Sell>>(++order_id, price, quantity));
  case "FAK"_hash:
    return !strcmp(side, "Buy") ? static_cast<OrderPtr>(std::make_shared<FillAndKillOrder<Side::Buy>>(++order_id, price, quantity))
                                : static_cast<OrderPtr>(std::make_shared<FillAndKillOrder<Side::Sell>>(++order_id, price, quantity));
  case "FOK"_hash:
    return !strcmp(side, "Buy") ? static_cast<OrderPtr>(std::make_shared<FillOrKillOrder<Side::Buy>>(++order_id, price, quantity))
                                : static_cast<OrderPtr>(std::make_shared<FillOrKillOrder<Side::Sell>>(++order_id, price, quantity));
  case "M"_hash:
    return !strcmp(side, "Buy") ? static_cast<OrderPtr>(std::make_shared<MarketOrder<Side::Buy>>(++order_id, price, quantity))
                                : static_cast<OrderPtr>(std::make_shared<MarketOrder<Side::Sell>>(++order_id, price, quantity));
  case "GFD"_hash:
    return !strcmp(side, "Buy") ? static_cast<OrderPtr>(std::make_shared<GoodForDayOrder<Side::Buy>>(++order_id, price, quantity))
                                : static_cast<OrderPtr>(std::make_shared<GoodForDayOrder<Side::Sell>>(++order_id, price, quantity));
  default:
    return nullptr;
  }
}
}
