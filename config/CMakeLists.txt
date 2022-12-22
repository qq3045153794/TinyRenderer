set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)



set(MAIN_LIBS
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/lib/libglfw3.a
    ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/bin/libassimp.so.5.2.0
    ${PROJECT_SOURCE_DIR}/vendor/stdlog/lib/libspdlog.a
    GL m Xrandr  X11  pthread dl
)

set(MAIN_SRC
    ${PROJECT_SOURCE_DIR}/src/asset/Buffer.cpp      
    ${PROJECT_SOURCE_DIR}/src/asset/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/Texture.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/VAO.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/FBO.cpp
    ${PROJECT_SOURCE_DIR}/src/core/App.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Window.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Clock.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Event.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Input.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Log.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Camera.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Transform.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Material.cpp
    ${PROJECT_SOURCE_DIR}/src/scene/Render.cpp
    ${PROJECT_SOURCE_DIR}/src/scene/Scene.cpp
    ${PROJECT_SOURCE_DIR}/src/scene/ui.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/File.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/image.cpp
)

set(ImGUI_SRC
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_impl_glfw.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include/imgui_impl_opengl3.cpp
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

set(ImGUI_INCLUDE
    ${PROJECT_SOURCE_DIR}/vendor/imgui/include
) 

set(MAIN_INCLUDE
    ${PROJECT_SOURCE_DIR}/src
)

set(VEN_INCLUDE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/std_image
    ${PROJECT_SOURCE_DIR}/vendor/EnTT/include
    ${PROJECT_SOURCE_DIR}/vendor/stdlog/include
)