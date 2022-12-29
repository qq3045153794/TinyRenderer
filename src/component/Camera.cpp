/*
 * @Author: xietao
 * @Date: 2022-08-03 20:22:11
 * @LastEditors: xietao
 * @LastEditTime: 2022-08-30 16:21:12
 * @FilePath: /Learn_OpenGL/src/component/Camera.cpp
 * @brief:
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#include "component/Camera.h"

#include <iostream>

#include "core/Clock.h"
#include "core/Input.h"
#include "core/Log.h"
namespace component {

using namespace core;

Camera::Camera(Transform* t, float fov, float aspect, float znear, float zfar)
    : T(t),
      m_fov(fov),
      m_aspect(aspect),
      m_znear(znear),
      m_zfar(zfar),
      m_camera_projection(PERSPECTIVE) {}

Camera::Camera(Transform* t, float left, float right, float bottom, float top, float znear,
               float zfar)
    : T(t),
      m_left(left),
      m_right(right),
      m_bottom(bottom),
      m_top(top),
      m_znear(znear),
      m_zfar(zfar),
      m_camera_projection(ORTHOGRAPHIC) {}

glm::mat4 Camera::get_projection_mat() const {
  if (m_camera_projection == PERSPECTIVE) {
    return glm::perspective(glm::radians(m_fov), m_aspect, m_znear, m_zfar);
  } else {
    return glm::ortho(m_left, m_right, m_bottom, m_top, m_znear, m_zfar);
  }
};

CameraFps::CameraFps(Transform* t, float fov, float aspect, float znear, float zfar)
    : Camera(t, fov, aspect, znear, zfar) {}

CameraFps::CameraFps(Transform* t, float left, float right, float bottom, float top, float znear,
                     float zfar)
    : Camera(t, left, right, bottom, top, znear, zfar) {}

void CameraFps::update() {
  float dt = Clock::dt;
  float velocity = dt;

  if (Input::get_key_down('w')) {
    T->translate(T->m_forward * velocity);
  }

  if (Input::get_key_down('s')) {
    T->translate(T->m_forward * velocity * -1.0f);
  }

  if (Input::get_key_down('a')) {
    T->translate(T->m_right * velocity * -1.0f);
  }

  if (Input::get_key_down('d')) {
    T->translate(T->m_right * velocity);
  }

  if (Input::get_key_down('q')) {
    CORE_TRACE("input q");
    T->translate(world_up * velocity);
  }

  if (Input::get_key_down('e')) {
    T->translate(world_up * velocity * -1.0f);
  }

  auto eular_x = glm::clamp(T->get_eular().x + Input::get_offset_y() * 0.1f, -89.0f, 89.0f);
  auto eular_y = T->get_eular().y + Input::get_offset_x() * 0.1;

  T->set_ealar_YZX(glm::vec3(eular_x, eular_y, 0.0f));
}

}  // namespace component