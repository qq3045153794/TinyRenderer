
add_executable(demo_glfw
  ${PROJECT_SOURCE_DIR}/examples/demo_GLFW/main.cpp
  ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

# 编译选项 https://blog.csdn.net/zhizhengguan/article/details/111743586
target_compile_options(demo_glfw
    # Diagnose
    PUBLIC -Wall
    # Debug
    PUBLIC -O0
    PUBLIC -ggdb
)


target_include_directories(demo_glfw
    PUBLIC
        ${VEN_INCLUDE}
)

target_link_libraries(demo_glfw
    PUBLIC
    ${VEN_LIBS}
)
