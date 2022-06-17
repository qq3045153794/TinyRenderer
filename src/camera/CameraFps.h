#pragma once
#include "ResourceManager.h"
namespace gl_cameras {

#define mat4_X(a) glm::mat4(1.0, 0.0, 0.0, 0.0, \
                       0.0, glm::cos(a), -glm::sin(a), 0.0, \
                       0.0, glm::sin(a),  glm::cos(a), 0.0, \
                       0.0, 0.0, 0.0, 1.0)

#define mat4_Y(b) glm::mat4(glm::cos(b), 0.0, glm::sin(b), 0.0, \
                       0.0, 1.0, 0.0, 0.0, \
                       -glm::sin(b), 0.0, glm::cos(b), 0.0, \
                       0.0, 0.0, 0.0, 1.0)

#define mat4_Z(y) glm::mat4(glm::cos(y), -glm::sin(y), 0.0, 0.0,\
                            glm::sin(y), glm::cos(y), 0.0, 0.0,\
                            0.0, 0.0, 1.0, 0.0,\
                            0.0, 0.0, 0.0, 1.0)

class CameraFps {
 public: 
  const glm::mat4& const getViewMatrix() { return glm::lookAt(eye_, eye_+front_, up_); }
 private:
  void updateCameraVector();
  
  GLfloat pitch_;
  GLfloat yaw_;
  GLfloat roll_;
  GLfloat zoom_;
  glm::vec3 front_;
  glm::vec3 eye_;
  glm::vec3 up_;
  glm::vec3 world_up_;
};

}