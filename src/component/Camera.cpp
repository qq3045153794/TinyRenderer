#include "component/Camera.h"

namespace component {

CameraFps::CameraFps(float fov, float aspect, float znear, float zfar) 
    : Camera(), 
      m_fov(fov),
      m_aspect(aspect),
      m_znear(znear),
      m_zfar(zfar) {

}

const glm::mat4& CameraFps::get_projection_mat() const {
  return glm::perspective(m_fov, m_aspect, m_znear, m_zfar);
}

}