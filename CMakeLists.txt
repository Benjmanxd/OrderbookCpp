cmake_minimum_required(VERSION 3.27)

project(
  Orderbook
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
add_subdirectory(src)

add_executable(orderbook main.cpp)
target_link_libraries(orderbook
  PRIVATE
  OrderbookLib)
