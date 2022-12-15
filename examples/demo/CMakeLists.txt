cmake_minimum_required(VERSION 3.15)
project(DEMO)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

include(${PROJECT_SOURCE_DIR}/config/CMakeLists.txt)

add_executable(demo
    ${PROJECT_SOURCE_DIR}/examples/demo/main.cpp
    ${MAIN_SRC}
    ${ImGUI_SRC}
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

target_include_directories(demo
    PUBLIC
        ${ImGUI_INCLUDE}
        ${MAIN_INCLUDE}
        ${VEN_INCLUDE}
)

target_link_libraries(demo
    PUBLIC
        ${MAIN_LIBS}
)