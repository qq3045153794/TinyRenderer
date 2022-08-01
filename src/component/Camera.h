#ifndef _LEARN_OPENGL_SRC_COMPONENT_CAMERA_H_
#define _LEARN_OPENGL_SRC_COMPONENT_CAMERA_H_

#include "component/Transform.h"

namespace component {

class Camera {
 public:
  Camera() : T(){};
  virtual ~Camera(){};

  const glm::mat4& get_view_mat() const { return T.get_lookat(); };

  virtual const glm::mat4& get_projection_mat() const = 0;
  virtual void update() = 0;

 protected:
  Transform T;
};

class CameraFps : public Camera {
 public:
  CameraFps(float fov, float aspect, float znear, float zfar);
  virtual ~CameraFps();
  virtual const glm::mat4& get_projection_mat() const override;
  virtual void update() override;

 private:
  float m_fov;
  float m_aspect;
  float m_znear;
  float m_zfar;
};

}  // namespace component

#endif