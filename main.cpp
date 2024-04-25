#include <cassert>
#include <iostream>
#include <memory>

#include "orderbook.h"

int main(void) {
  Orderbook orderbook;

  std::cout << "Test Add and Cancel: " << std::endl;
  orderbook.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 1,
                                             Side::Sell, 100, 100));
  assert(orderbook.Size() == 1);
  orderbook.CancelOrder(1);
  assert(orderbook.Size() == 0);
  std::cout << "Test passed" << std::endl;
  std::cout << std::endl;

  std::cout << "Test Fill: " << std::endl;
  orderbook.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 1,
                                             Side::Sell, 100, 100));
  assert(orderbook.Size() == 1);
  orderbook.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 2,
                                             Side::Buy, 100, 100));
  assert(orderbook.Size() == 0);
  std::cout << "Test passed" << std::endl;
  std::cout << std::endl;

  std::cout << "Test FillAndKill: " << std::endl;
  orderbook.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 1,
                                             Side::Sell, 100, 100));
  assert(orderbook.Size() == 1);
  orderbook.AddOrder(
      std::make_shared<Order>(OrderType::FillAndKill, 2, Side::Buy, 100, 100));
  assert(orderbook.Size() == 0);
  orderbook.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 1,
                                             Side::Sell, 100, 100));
  assert(orderbook.Size() == 1);
  orderbook.AddOrder(
      std::make_shared<Order>(OrderType::FillAndKill, 3, Side::Buy, 100, 50));
  assert(orderbook.Size() == 1);
  orderbook.AddOrder(
      std::make_shared<Order>(OrderType::FillAndKill, 4, Side::Buy, 100, 50));
  assert(orderbook.Size() == 0);
  std::cout << "Test passed" << std::endl;
  std::cout << std::endl;
}
