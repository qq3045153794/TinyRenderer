/*
 * @Author       : xietao
 * @Date         : 2022-07-28 16:38:56
 * @LastEditors  : xietao
 * @LastEditTime : 2022-08-30 18:49:46
 * @FilePath     : /Learn_OpenGL/src/component/Camera.h
 * @Description  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */

#ifndef _LEARN_OPENGL_SRC_COMPONENT_CAMERA_H_
#define _LEARN_OPENGL_SRC_COMPONENT_CAMERA_H_

#include "component/Transform.h"

namespace component {

class Camera {
 public:
  Camera(float fov, float aspect, float znear, float zfar);
  Camera(float left, float right, float bottom, float top, float znear, float zfar);
  virtual ~Camera(){};

  const glm::mat4 get_view_mat() const { return T.get_lookat(); };

  const glm::mat4 get_projection_mat() const;

  void set_position(const glm::vec3& position) { T.translate(position); }

  void set_eular(const glm::vec3& eular) { T.ealar_ratation(eular); };

 protected:
  enum CameraProjection { PERSPECTIVE, ORTHOGRAPHIC };

  CameraProjection m_camera_projection;
  Transform T;
  float m_fov;
  float m_aspect;

  float m_left, m_right, m_bottom, m_top;
  float m_znear;
  float m_zfar;
};

class CameraFps : public Camera {
 public:
  CameraFps(float fov, float aspect, float znear, float zfar);
  CameraFps(float left, float right, float bottom, float top, float znear, float zfar);
  
  virtual ~CameraFps(){};
  void update();

 private:
  float m_fov;
  float m_aspect;
  float m_znear;
  float m_zfar;
};

}  // namespace component

#endif