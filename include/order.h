#ifndef _ORDER_H_
#define _ORDER_H_

#include <memory>
#include <list>

#include "types.h"

class Order {
public:
  Order(OrderType order_type, OrderId order_id, Side side, Price price,
        Quantity quantity)
      : m_order_type(order_type), m_order_id(order_id), m_side(side),
        m_price(price), m_initial_quantity(quantity),
        m_remaining_quantity(quantity) {}

  OrderType GetOrderType() const { return m_order_type; }
  OrderId GetOrderId() const { return m_order_id; }
  Side GetSide() const { return m_side; }
  Price GetPrice() const { return m_price; }
  Quantity GetInitialQuantity() const { return m_initial_quantity; }
  Quantity GetRemainingQuantity() const { return m_remaining_quantity; }
  bool IsFilled() const { return m_remaining_quantity == 0; }
  bool Fill(Quantity quantity);

private:
  OrderType m_order_type;
  OrderId m_order_id;
  Side m_side;
  Price m_price;
  Quantity m_remaining_quantity;
  Quantity m_initial_quantity;
};

using OrderPtr = std::shared_ptr<Order>;
using OrderPtrs = std::list<OrderPtr>;

class OrderModify {
public:
  OrderModify(OrderId order_id, Side side, Price price, Quantity quantity)
      : m_order_id(order_id), m_side(side), m_price(price),
        m_quantity(quantity) {}

  OrderId GetOrderId() const { return m_order_id; }
  Side GetSide() const { return m_side; }
  Price GetPrice() const { return m_price; }
  Quantity GetQuantity() const { return m_quantity; }
  OrderPtr Convert(OrderType) const;

private:
  OrderId m_order_id;
  Side m_side;
  Price m_price;
  Quantity m_quantity;
};

#endif
