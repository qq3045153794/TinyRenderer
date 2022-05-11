包含头需要宏定义
...cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
...


注意官方说明
before you include this file in *one* C or C++ file to create the implementation.
意思是要在只能被包含一次