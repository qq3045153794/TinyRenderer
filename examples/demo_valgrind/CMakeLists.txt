project(DEMO_VAL VERSION 0.0.1 LANGUAGES C CXX)

include(${PROJECT_SOURCE_DIR}/config/CMakeLists.txt)

add_executable(demo_val
  ${PROJECT_SOURCE_DIR}/examples/demo_valgrind/main.cpp
  ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

# 编译选项 https://blog.csdn.net/zhizhengguan/article/details/111743586
target_compile_options(demo_val
    # Diagnose
    PUBLIC -Wall
    # Debug
    PUBLIC -O0
    PUBLIC -ggdb
)


target_include_directories(demo_val
    PUBLIC
      ${PROJECT_SOURCE_DIR}/vendor/glad/include
)


target_link_libraries(demo_val
  PUBLIC
     dl
)
