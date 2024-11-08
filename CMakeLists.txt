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
file(GLOB IMGUI_SRC_FILES ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)

# include core header, source files
include_directories(include)
add_subdirectory(src)

add_executable(orderbook main.cpp ${IMGUI_SRC_FILES})
target_link_libraries(orderbook
  PRIVATE
  OrderbookLib
  OpenGL
  glfw)
