add_executable(saber
    main.cpp
    ${MAIN_SRC}
    ${ImGUI_SRC}
    #${ImGuizmo_SRC}
    ${PROJECT_SOURCE_DIR}/vendor/glad/src/glad.c
)

# 编译选项 https://blog.csdn.net/zhizhengguan/article/details/111743586
target_compile_options(saber
    # Diagnose
    PUBLIC -Wall
    # Debug
    PUBLIC -O0
    PUBLIC -ggdb
)


target_include_directories(saber
    PUBLIC
        ${ImGUI_INCLUDE}
        ${MAIN_INCLUDE}
        #${ImGuizmo_INCLUDE}
        ${VEN_INCLUDE}
)

target_link_libraries(saber
    PUBLIC
      ${VEN_LIBS}
)
