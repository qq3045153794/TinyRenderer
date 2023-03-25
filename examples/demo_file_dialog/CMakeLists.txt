
add_executable(file_dialog
    main.cpp
    ${ImGUI_SRC}
    ${IMGUI_FILEDIALOG_SRC}
)

# 编译选项 https://blog.csdn.net/zhizhengguan/article/details/111743586
target_compile_options(file_dialog

    # Diagnose
    PUBLIC -Wall
    # Debug
    PUBLIC -O0
    PUBLIC -ggdb
)


target_include_directories(file_dialog
    PUBLIC
        ${ImGUI_INCLUDE}
        ${IMGUI_FILEDIALOG_INCLUDE}
)

target_link_libraries(file_dialog
    PUBLIC
      ${VEN_LIBS}
)
