cmake_minimum_required(VERSION 3.27)

option(BUILD_NIX "build in nixos" OFF)
if (BUILD_NIX)
  set(CMAKE_CXX_COMPILER "/etc/profiles/per-user/$ENV{USER}/bin/g++")
  set(CMAKE_C_COMPILER "/etc/profiles/per-user/$ENV{USER}/bin/gcc")
endif()

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
