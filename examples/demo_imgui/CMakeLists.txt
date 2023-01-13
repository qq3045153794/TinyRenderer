
project(DemoImGUI)

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
