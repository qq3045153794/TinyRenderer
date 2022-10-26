cmake_minimum_required(VERSION 3.15)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


set(LIBS
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/lib/libglfw3.a
    ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/bin/libassimp.so.5.2.0
    GL m Xrandr  X11  pthread dl
)


add_executable(demo_9
    ${PROJECT_SOURCE_DIR}/examples/demo9/main.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/Buffer.cpp      
    ${PROJECT_SOURCE_DIR}/src/asset/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/Texture.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/VAO.cpp
    ${PROJECT_SOURCE_DIR}/src/core/App.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Window.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Clock.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Event.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Input.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Camera.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Transform.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Material.cpp
    ${PROJECT_SOURCE_DIR}/src/scene/Render.cpp
    ${PROJECT_SOURCE_DIR}/src/scene/Scene.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/File.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/image.cpp
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

target_include_directories(demo_9
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/std_image
    ${PROJECT_SOURCE_DIR}/vendor/EnTT/include
    
)

target_link_libraries(demo_9
    PUBLIC
        ${LIBS}
)