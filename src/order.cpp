#include "order.h"

#include <stdexcept>

void Order::Fill(Quantity quantity) {
  if (quantity > m_remaining_quantity)
    throw std::logic_error("Order cannot be filled more than its remaining quantity");

  m_remaining_quantity -= quantity;
}

OrderPtr OrderModify::Convert(OrderType order_type) const {
  return std::make_shared<Order>(order_type, m_order_id, m_side, m_price, m_quantity);
}

void Order::PriceAdjust(Price price) {
  if (m_order_type != OrderType::Market)
    throw std::logic_error("Only market orders can adjust price");

  m_order_type = OrderType::GoodTillCancel;
  m_price = price;
}
