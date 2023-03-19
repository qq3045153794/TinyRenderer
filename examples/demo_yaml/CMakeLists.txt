
add_executable(demo_yaml
    main.cpp
)

# 编译选项 https://blog.csdn.net/zhizhengguan/article/details/111743586
target_compile_options(demo_yaml
    # Diagnose
    PUBLIC -Wall
    # Debug
    PUBLIC -O0
    PUBLIC -ggdb
)


target_include_directories(demo_yaml
  PUBLIC
    ${YAML-CPP_INCLUDE}
)

target_link_libraries(demo_yaml
  PUBLIC
    ${YAML-CPP_LIBS}
)
