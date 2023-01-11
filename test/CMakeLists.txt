project(test VERSION 0.0.1 LANGUAGES C CXX)


include(${PROJECT_SOURCE_DIR}/config/CMakeLists.txt)

add_executable(test
    ${PROJECT_SOURCE_DIR}/test/TransformTest.cpp
    ${MAIN_SRC}
    ${ImGUI_SRC}
    ${ImGuizmo_SRC}
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

# 编译选项 https://blog.csdn.net/zhizhengguan/article/details/111743586
target_compile_options(test
    # Diagnose
    PUBLIC -Wall
    # Debug
    PUBLIC -O0
    PUBLIC -ggdb
)


target_include_directories(test
    PUBLIC
        ${ImGUI_INCLUDE}
        ${MAIN_INCLUDE}
        ${ImGuizmo_INCLUDE}
        ${VEN_INCLUDE}
        ${GTEST_INCLUDE}
)

target_link_libraries(test
    PUBLIC
        ${MAIN_LIBS}
        ${GTEST_LIB}
)
