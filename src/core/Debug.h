#ifndef _SRC_CORE_DEBUG_H_
#define _SRC_CORE_DEBUG_H_

#include <string>

#include "core/Log.h"
#include "glad/glad.h"

namespace core {

class Debug {
 public:
  static std::string gl_check_err(GLenum error_code) {
    std::string error = "";
    switch (error_code) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
      default:
        error = "UNKOWN ERROR";
    }

    return error;
  }
};
}  // namespace core

#define CHECK_ERROR()                                          \
  {                                                            \
    GLenum error_code = glGetError();                          \
    if (error_code  != GL_NO_ERROR) {                         \
      CORE_ERROR("{}", core::Debug::gl_check_err(error_code)); \
    }                                                          \
  }

#endif
