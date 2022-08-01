#include "component/Camera.h"
#include "core/Clock.h"
#include "core/Input.h"
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

void CameraFps::update() {
  
  float dt = Clock::dt;
  float velocity = dt;
  
  if(Input::get_key_down('w')) {
    T.translate(T.m_forward * velocity);
  } else if(Input::get_key_down('s')) {
    T.translate(T.m_forward * velocity * -1.0f);
  } else if(Input::get_key_down('a')) {
    T.translate(T.m_right * velocity * -1.0f);
  } else if(Input::get_key_down('d')) {
    T.translate(T.m_right * velocity);
  }

}


}