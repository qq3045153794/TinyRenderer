#include "CameraFps.h"
namespace gl_cameras {

const GLfloat kPitch = 0.0f;
const GLfloat kYaw = 0.0f;
const GLfloat kRoll = 0.0f;
const GLfloat kZoom = 45.0f;

const GLfloat kSpeed = 2.5f;
const GLfloat kSensitivity = 0.05f;

CameraFps::CameraFps(glm::vec3 eye, glm::vec3 front, glm::vec3 up)
    : pitch_(kPitch),
      yaw_(kYaw),
      roll_(kRoll),
      zoom_(kZoom),
      movement_speed_(kSpeed),
      mouse_sensitivity_(kSensitivity),
      eye_(eye),
      front_(front),
      up_(up),
      right_(glm::cross(front, up)),
      kFront(front),
      kUp(up),
      kRight(glm::cross(front, up)) {
  updateCameraVector();
}

void CameraFps::updateCameraVector() {
  front_ = mat4_Y(glm::radians(-yaw_)) * mat4_Z(glm::radians(pitch_)) *
           mat4_X(glm::radians(roll_)) * glm::vec4(kFront, 0.0);
  up_ = mat4_Y(glm::radians(-yaw_)) * mat4_Z(glm::radians(pitch_)) * mat4_X(glm::radians(roll_)) *
        glm::vec4(kUp, 0.0);
  right_ = mat4_Y(glm::radians(-yaw_)) * mat4_Z(glm::radians(pitch_)) *
           mat4_X(glm::radians(roll_)) * glm::vec4(kRight, 0.0);

}

void CameraFps::processKeyboard(CameraMovement direction, float delta_time) {
  GLfloat velocity = movement_speed_ * delta_time;
  if (direction == FORWARD)
    eye_ += front_ * velocity;
  else if (direction == BACKWARD)
    eye_ -= front_ * velocity;
  else if (direction == LEFT)
    eye_ -= right_ * velocity;
  else if (direction == RIGHT)
    eye_ += right_ * velocity;
  updateCameraVector();
}

void CameraFps::processMouseMovement(GLfloat xoffset, GLfloat yoffset) {
  xoffset *= mouse_sensitivity_;
  yoffset *= mouse_sensitivity_;
  yaw_ += xoffset;
  pitch_ += yoffset;

  if (pitch_ > 89.0) pitch_ = 89.0f;

  if (pitch_ < -89.0) pitch_ = -89.0f;

  updateCameraVector();
}

void CameraFps::processMouseScroll(GLfloat yoffset) {
  zoom_ -= static_cast<GLfloat>(yoffset);
  if (zoom_ < 1.0f) zoom_ = 1.0;
  if (zoom_ > 45.0f) zoom_ = 45.0f;
}

}  // namespace gl_cameras