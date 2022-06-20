cmake_minimum_required(VERSION 3.15)
project(LearnOpenGL)
set(SRC
    
    ${PROJECT_SOURCE_DIR}/src/model/Trangle.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/ResourceManager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/Texture2D.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/RenderText.cpp
    ${PROJECT_SOURCE_DIR}/src/camera/CameraFps.cpp
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

set(LIBS
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/lib/libglfw3.a
    ${PROJECT_SOURCE_DIR}/vendor/std_image/stb_image.h
    ${PROJECT_SOURCE_DIR}/vendor/freetype/lib/libfreetype.so.6.17.0
    GL m Xrandr  X11  pthread dl
)

add_executable(${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/examples/demo1/main.cpp
    ${SRC}      
)
#add_executable(${PROJECT_NAME}
#    ${PROJECT_SOURCE_DIR}/src/tools/front.cpp
#    ${SRC}
#)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/model
        ${PROJECT_SOURCE_DIR}/src/tools
        ${PROJECT_SOURCE_DIR}/src/camera
        ${PROJECT_SOURCE_DIR}/vendor/glad/include
        ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/std_image
        ${PROJECT_SOURCE_DIR}/vendor/freetype/include/freetype2
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${LIBS}
)