#include "Camera.h"

namespace gl_cameras {

Camera::Camera(float fov, float aspect, float near, float far) {
  projection_mat_ = glm::perspective(glm::radians(fov), aspect, near, far);
}

void Camera::update() {
  view_mat_ = glm::lookAt(eye_, center_, up_);
}

void Camera::set_eye(const glm::vec3& e) {
  eye_ = e;
  update();
}

void Camera::set_center(const glm::vec3& c) {
  center_ = c;
  update();
}

void Camera::set_up(const glm::vec3& u) {
  up_ = u;
  update();
}

void Camera::look_at(const glm::vec3& e, const glm::vec3& c, const glm::vec3& u) {
  eye_ = e;
  center_ = c;
  up_ = u;
  update();
}

const glm::mat4& Camera::get_view_mat() {
  return view_mat_;
}

const glm::mat4& Camera::get_projection_mat() {
  return projection_mat_;
}

const glm::vec3& Camera::get_eye() {
  return eye_;
}
const glm::vec3& Camera::get_center() {
  return center_;
}
const glm::vec3& Camera::get_up() {
  return up_;
}

}