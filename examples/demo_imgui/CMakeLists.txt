cmake_minimum_required(VERSION 3.15)
project(DemoImGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


include(config/CMakeLists.txt)

add_executable(DemoImGUI
    ${PROJECT_SOURCE_DIR}/examples/demo_imgui/main.cpp
    ${ImGUI_SRC}
)

target_include_directories(DemoImGUI
    PUBLIC
        ${ImGUI_INCLUDE}
        ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
)

set(LIBS
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/lib/libglfw3.a
    GL m Xrandr  X11  pthread dl
)


target_link_libraries(DemoImGUI
    PUBLIC
        ${LIBS}
)