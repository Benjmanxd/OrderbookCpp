cmake_minimum_required(VERSION 3.5)

project(
  Orderbook
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "CMAKE Version: ${CMAKE_VERSION}")
message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Host System name: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "System version: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

set(CMAKE_NO_WARN_FLAGS "${CMAKE_NO_WARN_FLAGS} -Wno-missing-field-initializers")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_NO_WARN_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -Werror -O3 -fstack-protector-all -fPIE")

# make sure openGL, GLFW library installed in system
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include(FetchContent)
# fetch imgui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.4-docking)
FetchContent_MakeAvailable(imgui)
message("imgui source directory is :" ${imgui_SOURCE_DIR})
include_directories(${imgui_SOURCE_DIR}/)
include_directories(${imgui_SOURCE_DIR}/backends)
file(GLOB IMGUI_SRC_FILES ${imgui_SOURCE_DIR}/*.cpp
     ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
     ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

# include core header, source files
include_directories(include)
add_subdirectory(src)

add_executable(orderbook main.cpp ${IMGUI_SRC_FILES})
target_link_libraries(orderbook PRIVATE OrderbookApp OrderbookCore OpenGL glfw)
