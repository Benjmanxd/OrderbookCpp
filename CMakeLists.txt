cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_COMPILER "/etc/profiles/per-user/benjmanxd/bin/g++")
set(CMAKE_C_COMPILER "/etc/profiles/per-user/benjmanxd/bin/gcc")

project(
  Orderbook
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(OrderbookLib orderbook.h orderbook.cpp)
add_executable(orderbook main.cpp)
target_link_libraries(orderbook
  PRIVATE
  OrderbookLib)
