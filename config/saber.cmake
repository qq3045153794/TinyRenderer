set(ImGUI_SRC
    #${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_demo.cpp
    #${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui.cpp
    #${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_draw.cpp
    #${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_tables.cpp
    #${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_widgets.cpp
    #${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_impl_glfw.cpp
    #${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_impl_opengl3.cpp

    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/imgui.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/backends/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/backends/imgui_impl_opengl3.cpp
)


set(ImGUI_INCLUDE
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/
    ${PROJECT_SOURCE_DIR}/vendor/imgui-docking/backends/
)

set(ImGuizmo_SRC
    ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/include/ImGuizmo/GraphEditor.cpp
    ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/include/ImGuizmo/ImCurveEdit.cpp
    ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/include/ImGuizmo/ImGradient.cpp
    ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/include/ImGuizmo/ImGuizmo.cpp
    ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/include/ImGuizmo/ImSequencer.cpp
)

set(ImGuizmo_INCLUDE
    ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/include
)


set(GTEST_INCLUDE
  ${PROJECT_SOURCE_DIR}/vendor/googletest/ubuntu-20.04/include
)

set(GTEST_LIB
  ${PROJECT_SOURCE_DIR}/vendor/googletest/ubuntu-20.04/lib/libgmock.a

  ${PROJECT_SOURCE_DIR}/vendor/googletest/ubuntu-20.04/lib/libgmock_main.a

  ${PROJECT_SOURCE_DIR}/vendor/googletest/ubuntu-20.04/lib/libgtest.a

  ${PROJECT_SOURCE_DIR}/vendor/googletest/ubuntu-20.04/lib/libgtest_main.a
)

set(VEN_INCLUDE
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/std_image
    ${PROJECT_SOURCE_DIR}/vendor/EnTT/include
    ${PROJECT_SOURCE_DIR}/vendor/stdlog/include
)

set(VEN_LIBS
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/lib/libglfw3.a
    ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/bin/libassimp.so.5.2.0
    ${PROJECT_SOURCE_DIR}/vendor/stdlog/lib/libspdlog.a
    GL m Xrandr  X11  pthread dl
)

set(MAIN_INCLUDE
    ${PROJECT_SOURCE_DIR}/src
)


set(YAML-CPP_INCLUDE
  ${PROJECT_SOURCE_DIR}/vendor/yaml-cpp/include
)

set(YAML-CPP_LIBS
  ${PROJECT_SOURCE_DIR}/vendor/yaml-cpp/lib/libyaml-cpp.a
)

FILE(GLOB_RECURSE MAIN_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(RresourceDir ${PROJECT_SOURCE_DIR}/resource)
