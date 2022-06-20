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

enum CameraMovement{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class CameraFps {
 public: 
  CameraFps (glm::vec3 eye, glm::vec3 front, glm::vec3 up);
  const glm::mat4 getViewMatrix() { return glm::lookAt(eye_, eye_+front_, up_); }
  const GLfloat & getFov() { return zoom_; }
  void processKeyboard(CameraMovement direction, float deltaTime);
  void processMouseMovement(GLfloat xoffset, GLfloat yoffset);
  void processMouseScroll(GLfloat yoffset);
 private:
  void updateCameraVector();
  
  GLfloat pitch_;
  GLfloat yaw_;
  GLfloat roll_;
  GLfloat zoom_;
  
  GLfloat movement_speed_;
  GLfloat mouse_sensitivity_;

  const glm::vec3 kFront;
  const glm::vec3 kUp;
  const glm::vec3 kRight;

  glm::vec3 eye_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
};

}