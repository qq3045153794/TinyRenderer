project(DEMO VERSION 0.0.1 LANGUAGES C CXX)


include(${PROJECT_SOURCE_DIR}/config/CMakeLists.txt)

add_executable(demo
    ${PROJECT_SOURCE_DIR}/examples/demo/main.cpp
    ${MAIN_SRC}
    ${ImGUI_SRC}
    ${ImGuizmo_SRC}
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

# 编译选项 https://blog.csdn.net/zhizhengguan/article/details/111743586
target_compile_options(demo
    # Diagnose
    PUBLIC -Wall
    # Debug
    PUBLIC -O0
    PUBLIC -ggdb
)


target_include_directories(demo
    PUBLIC
        ${ImGUI_INCLUDE}
        ${MAIN_INCLUDE}
        ${ImGuizmo_INCLUDE}
        ${VEN_INCLUDE}
)

target_link_libraries(demo
    PUBLIC
        ${MAIN_LIBS}
)