#include "CameraFps.h"
namespace gl_cameras {
  
void CameraFps::updateCameraVector() {
  front_ = mat4_Y(glm::radians(-yaw_)) * mat4_Z(glm::radians(pitch_)) * mat4_X(glm::radians(roll_)) * glm::vec4(front_, 0.0);
  up_ = mat4_Y(glm::radians(-yaw_)) * mat4_Z(glm::radians(pitch_)) * mat4_X(glm::radians(roll_)) * glm::vec4(up_, 0.0); 
  
}

}