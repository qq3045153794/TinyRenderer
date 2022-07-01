#ifndef _LEARN_OPENGL_SRC_ANIMATION_H_
#define _LEARN_OPENGL_SRC_ANIMATION_H_
#include "glm/glm.hpp"
namespace gl_animation {

struct BoneInfo {
  int id;
  glm::mat4 offset;
};

}

#endif