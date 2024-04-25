#include "order.h"

#include <iostream>

bool Order::Fill(Quantity quantity) {
  if (quantity > m_remaining_quantity) {
    std::cerr << "Order " << m_order_id
              << "cannot be filled more than its remaining quantity"
              << std::endl;
    return false;
  }

  m_remaining_quantity -= quantity;
  return true;
}

OrderPtr OrderModify::Convert(OrderType order_type) const {
  return std::make_shared<Order>(order_type, m_order_id, m_side, m_price,
                                 m_quantity);
}
