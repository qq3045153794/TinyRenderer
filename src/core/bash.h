#ifndef _SRC_CORE_BASH_H_
#define _SRC_CORE_BASH_H_

#ifdef _WIN32
#include <direct.h>
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <unistd.h>
#endif

#endif
