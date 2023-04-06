/*
 * @Author       : xietao
 * @Date         : 2022-07-28 16:38:56
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-22 17:01:47
 * @FilePath     : /Learn_OpenGL/src/component/Camera.h
 * @brief  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */

#ifndef _LEARN_OPENGL_SRC_COMPONENT_CAMERA_H_
#define _LEARN_OPENGL_SRC_COMPONENT_CAMERA_H_

#include <scene/SerializeEntity.h>
#include "component/Transform.h"

namespace component {

class Camera {
 public:
  friend ::scene::SerializeObject;
  Camera(Transform* T, float fov, float aspect, float znear, float zfar);
  Camera(Transform* T, float left, float right, float bottom, float top, float znear, float zfar);
  virtual ~Camera(){};

  glm::mat4 get_view_mat() const { return T->get_lookat(); };

  glm::mat4 get_projection_mat() const;

  void set_position(const glm::vec3& position) { T->translate(position); }

  void set_eular(const glm::vec3& eular) { T->set_ealar_YZX(eular); };

  float& Fov();
  float& Aspect();
  float& Znear();
  float& Zfar();

  Transform* T;

  enum CameraProjection { PERSPECTIVE, ORTHOGRAPHIC };
 protected:

  CameraProjection m_camera_projection;

  float m_fov;
  float m_aspect;

  float m_left, m_right, m_bottom, m_top;
  float m_znear;
  float m_zfar;
};

class CameraFps : public Camera {
 public:
  CameraFps(Transform* T, float fov, float aspect, float znear, float zfar);
  CameraFps(Transform* T, float left, float right, float bottom, float top, float znear, float zfar);

  virtual ~CameraFps(){};
  void update();
};

}  // namespace component

#endif
