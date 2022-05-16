cmake_minimum_required(VERSION 3.15)
project(LearnOpenGL)
set(SRC
    ${PROJECT_SOURCE_DIR}/examples/demo1/main.cpp
    ${PROJECT_SOURCE_DIR}/src/model/Trangle.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/ResourceManager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/Texture2D.cpp
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

set(LIBS
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/lib/libglfw3.a
    ${PROJECT_SOURCE_DIR}/vendor/std_image/stb_image.h
    GL m Xrandr  X11  pthread dl
)

add_executable(${PROJECT_NAME} 
    ${SRC}      
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/model
        ${PROJECT_SOURCE_DIR}/src/tools
        ${PROJECT_SOURCE_DIR}/vendor/glad/include
        ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/std_image
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${LIBS}
)