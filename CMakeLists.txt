cmake_minimum_required(VERSION 3.15)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(SRC    
    ${PROJECT_SOURCE_DIR}/src/model/Trangle.cpp
    ${PROJECT_SOURCE_DIR}/src/model/Image.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/ResourceManager.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/Texture2D.cpp
    ${PROJECT_SOURCE_DIR}/src/tools/TextureCube.cpp
    #${PROJECT_SOURCE_DIR}/src/tools/RenderText.cpp
    ${PROJECT_SOURCE_DIR}/src/camera/Camera.cpp
    ${PROJECT_SOURCE_DIR}/src/camera/CameraFps.cpp
    ${PROJECT_SOURCE_DIR}/src/camera/TrackBallControls.cpp
    ${PROJECT_SOURCE_DIR}/src/camera/TrackBallInteractor.cpp
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
    ${PROJECT_SOURCE_DIR}/src/frame/YUVFrame.cpp
    ${PROJECT_SOURCE_DIR}/src/render/TexCubeRender.cpp
)

set(LIBS
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/lib/libglfw3.a
    ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/bin/libassimp.so.5.2.0
    GL m Xrandr  X11  pthread dl
)

add_executable(demo_1 
    ${PROJECT_SOURCE_DIR}/examples/demo1/main.cpp
    ${SRC}      
)


target_include_directories(demo_1
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/model
        ${PROJECT_SOURCE_DIR}/src/tools
        ${PROJECT_SOURCE_DIR}/src/camera
        ${PROJECT_SOURCE_DIR}/src/animation
        ${PROJECT_SOURCE_DIR}/vendor/glad/include
        ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/std_image
        #${PROJECT_SOURCE_DIR}/vendor/freetype/include/freetype2
        ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/include
)

target_link_libraries(demo_1
    PUBLIC
        ${LIBS}
)


add_executable(demo_2 
    ${PROJECT_SOURCE_DIR}/examples/demo2/main.cpp
    ${SRC}      
)

target_include_directories(demo_2
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/model
        ${PROJECT_SOURCE_DIR}/src/tools
        ${PROJECT_SOURCE_DIR}/src/camera
        ${PROJECT_SOURCE_DIR}/src/animation
        ${PROJECT_SOURCE_DIR}/src/frame
        ${PROJECT_SOURCE_DIR}/vendor/glad/include
        ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/std_image
        ${PROJECT_SOURCE_DIR}/vendor/freetype/include/freetype2
        ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/include
        
)

target_link_libraries(demo_2
    PUBLIC
        ${LIBS}
)

add_executable(demo_3 
    ${PROJECT_SOURCE_DIR}/examples/demo3/main.cpp
    ${SRC}      
)

target_include_directories(demo_3
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/model
        ${PROJECT_SOURCE_DIR}/src/tools
        ${PROJECT_SOURCE_DIR}/src/camera
        ${PROJECT_SOURCE_DIR}/src/animation
        ${PROJECT_SOURCE_DIR}/src/frame
        ${PROJECT_SOURCE_DIR}/vendor/glad/include
        ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/std_image
        ${PROJECT_SOURCE_DIR}/vendor/freetype/include/freetype2
        ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/include
        
)

target_link_libraries(demo_3
    PUBLIC
        ${LIBS}
)


add_executable(demo_4 
    ${PROJECT_SOURCE_DIR}/examples/demo4/main.cpp
    ${SRC}      
)

target_include_directories(demo_4
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src/model
        ${PROJECT_SOURCE_DIR}/src/tools
        ${PROJECT_SOURCE_DIR}/src/camera
        ${PROJECT_SOURCE_DIR}/src/animation
        ${PROJECT_SOURCE_DIR}/src/frame
        ${PROJECT_SOURCE_DIR}/src/render
        ${PROJECT_SOURCE_DIR}/vendor/glad/include
        ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
        ${PROJECT_SOURCE_DIR}/vendor/glm
        ${PROJECT_SOURCE_DIR}/vendor/std_image
        ${PROJECT_SOURCE_DIR}/vendor/freetype/include/freetype2
        ${PROJECT_SOURCE_DIR}/vendor/assimp_ubuntu/include
        
)

target_link_libraries(demo_4
    PUBLIC
        ${LIBS}
)


add_executable(demo_5 
    ${PROJECT_SOURCE_DIR}/examples/demo5/main.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/Buffer.cpp      
    ${PROJECT_SOURCE_DIR}/src/asset/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/Texture.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/VAO.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/File.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/image.cpp
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

target_include_directories(demo_5
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/std_image
    
)

target_link_libraries(demo_5
    PUBLIC
        ${LIBS}
)


add_executable(demo_6 
    ${PROJECT_SOURCE_DIR}/examples/demo6/main.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/Buffer.cpp      
    ${PROJECT_SOURCE_DIR}/src/asset/Shader.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/Texture.cpp
    ${PROJECT_SOURCE_DIR}/src/asset/VAO.cpp
    ${PROJECT_SOURCE_DIR}/src/core/App.cpp
    ${PROJECT_SOURCE_DIR}/src/core/Window.cpp
    ${PROJECT_SOURCE_DIR}/src/component/Mesh.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/File.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/image.cpp
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

target_include_directories(demo_6
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/vendor/glad/include
    ${PROJECT_SOURCE_DIR}/vendor/glfw-3.3.7/include
    ${PROJECT_SOURCE_DIR}/vendor/glm
    ${PROJECT_SOURCE_DIR}/vendor/std_image
    
)

target_link_libraries(demo_6
    PUBLIC
        ${LIBS}
)