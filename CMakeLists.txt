cmake_minimum_required(VERSION 3.15)
project(DemoImGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(${PROJECT_SOURCE_DIR}/examples/demo_imgui/CMakeLists.txt)

include(${PROJECT_SOURCE_DIR}/examples/demo/CMakeLists.txt)