#pragma once

#include <list>
#include <memory>
#include <vector>

#include "types.h"

namespace OrderbookCore {
class OrderInterface {
public:
  virtual OrderType GetOrderType() const = 0;
  virtual Side GetSide() const = 0;
  virtual OrderId GetOrderId() const = 0;
  virtual Price GetPrice() const = 0;
  virtual Quantity GetInitialQuantity() const = 0;
  virtual Quantity GetRemainingQuantity() const = 0;
  virtual bool IsFilled() const = 0;

  virtual void Fill(Quantity) = 0;
  virtual void PriceAdjust(Price) = 0;
};

using OrderIds = std::vector<OrderId>;
using OrderPtr = std::shared_ptr<OrderInterface>;
using OrderPtrs = std::list<OrderPtr>;

template <Side side> class GoodTillCancelOrder;
template <Side side> class FillAndKillOrder;
template <Side side> class FillOrKillOrder;
template <Side side> class MarketOrder;
template <Side side> class GoodForDayOrder;

template <typename order_class, Side side> class Order : public OrderInterface {
public:
  Order(OrderId order_id, Price price, Quantity quantity)
      : m_order_id(order_id), m_price(price), m_initial_quantity(quantity), m_remaining_quantity(quantity) {}

  OrderType GetOrderType() const override { return m_order_type; }
  Side GetSide() const override { return m_side; }
  OrderId GetOrderId() const override { return m_order_id; }
  Price GetPrice() const override { return m_price; }
  Quantity GetInitialQuantity() const override { return m_initial_quantity; }
  Quantity GetRemainingQuantity() const override { return m_remaining_quantity; }
  bool IsFilled() const override { return m_remaining_quantity == 0; }

  void Fill(Quantity) override;
  void PriceAdjust(Price) override;

private:
  OrderType m_order_type = std::is_same<order_class, GoodTillCancelOrder<side>>() ? OrderType::GoodTillCancel
                           : std::is_same<order_class, FillAndKillOrder<side>>()  ? OrderType::FillAndKill
                           : std::is_same<order_class, FillOrKillOrder<side>>()   ? OrderType::FillOrKill
                           : std::is_same<order_class, MarketOrder<side>>()       ? OrderType::Market
                           : std::is_same<order_class, GoodForDayOrder<side>>()   ? OrderType::GoodForDay
                                                                                  : OrderType::Unknown;
  static constexpr Side m_side = side;
  OrderId m_order_id;
  Price m_price;
  Quantity m_initial_quantity;
  Quantity m_remaining_quantity;
};

template <Side side> class GoodTillCancelOrder : public Order<GoodTillCancelOrder<side>, side> {
public:
  GoodTillCancelOrder(OrderId order_id, Price price, Quantity quantity) : Order<GoodTillCancelOrder<side>, side>(order_id, price, quantity) {}
};

template <Side side> class FillAndKillOrder : public Order<FillAndKillOrder<side>, side> {
public:
  FillAndKillOrder(OrderId order_id, Price price, Quantity quantity) : Order<FillAndKillOrder<side>, side>(order_id, price, quantity) {}
};

template <Side side> class FillOrKillOrder : public Order<FillOrKillOrder<side>, side> {
public:
  FillOrKillOrder(OrderId order_id, Price price, Quantity quantity) : Order<FillOrKillOrder<side>, side>(order_id, price, quantity) {}
};

template <Side side> class MarketOrder : public Order<MarketOrder<side>, side> {
public:
  MarketOrder(OrderId order_id, Price price, Quantity quantity) : Order<MarketOrder<side>, side>(order_id, price, quantity) {}
};

template <Side side> class GoodForDayOrder : public Order<GoodForDayOrder<side>, side> {
public:
  GoodForDayOrder(OrderId order_id, Price price, Quantity quantity) : Order<GoodForDayOrder<side>, side>(order_id, price, quantity) {}
};

template <typename order_class> class OrderModify {
public:
  OrderModify(OrderId order_id, Price price, Quantity quantity) : m_order_id(order_id), m_price(price), m_quantity(quantity) {}

  OrderId GetOrderId() const { return m_order_id; }
  Price GetPrice() const { return m_price; }
  Quantity GetQuantity() const { return m_quantity; }
  OrderPtr Convert() const;

private:
  OrderId m_order_id;
  Price m_price;
  Quantity m_quantity;
};

class OrderFactory {
public:
  static OrderPtr CreateOrder(const char *side, const char *type, Quantity quantity, Price price);
};
}
