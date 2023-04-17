cmake_minimum_required(VERSION 3.15)

# 设置编译器
# set(CMAKE_C_COMPILER clang)
# set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)


# 设置cpp标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


project(TinyRenderer VERSION 0.0.1 LANGUAGES C CXX)

include(config/saber.cmake)

add_subdirectory(src)
add_subdirectory(examples/demo_yaml)
add_subdirectory(examples/demo_file_dialog)
add_subdirectory(examples/demo_GLFW)

file(COPY ${RresourceDir} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
