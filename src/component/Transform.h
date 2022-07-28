#ifndef _LEARN_OPENGL_SRC_COMPONENT_H_
#define _LEARN_OPENGL_SRC_COMPONENT_H_
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace component {

class Transform {
 public:
  
  void rotation(const glm::vec3& axis, float angle);
  void rotation(const glm::quat& q);
  void rotation(const glm::vec3& eular);
  
  void translate(const glm::vec3& v);
  void scale(const glm::vec3& s);
  const glm::mat4& get_transform() const;
  const glm::mat4& get_eular() const;
  const glm::mat4& get_position() const; 
  Transform();
  ~Transform();
 private:
  // 变换矩阵
  glm::mat4 m_transform;

  glm::vec3 m_position;
  glm::vec3 m_scale;
  glm::vec3 m_eular;
  glm::quat m_rotation;

  // 局部坐标
  glm::vec3 m_right, m_up, m_forward;
};

}

#endif