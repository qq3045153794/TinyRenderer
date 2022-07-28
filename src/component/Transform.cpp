#include "component/Transform.h"

namespace component {

static const glm::vec3 world_right{1.0f, 0.0f, 0.0f};     // +x axis
static const glm::vec3 world_up{0.0f, 1.0f, 0.0f};        // +y axis
static const glm::vec3 world_forward{0.0f, 0.0f, -1.0f};  // -z axis

Transform::Transform()
    : m_transform(1.0),
      m_position(1.0),
      m_eular(0.0),
      m_rotation(1.0, 0.0, 0.0, 0.0),
      m_scale(1.0),
      m_right(world_right),
      m_up(world_up),
      m_forward(world_forward) {}

Transform::~Transform() {}

void Transform::translate(const glm::vec3& v) {
  m_position = m_position + v;
  m_transform[3] = glm::vec4(m_position, 1.0f);
}

void Transform::scale(const glm::vec3& s) {
  m_scale = m_scale + s;
  m_transform[0][0] = m_scale.x;
  m_transform[1][1] = m_scale.y;
  m_transform[2][2] = m_scale.z;
  m_transform[3][3] = 1.0f;
}

void Transform::rotation(const glm::vec3& axis, float angle) {
  glm::quat q = glm::angleAxis(angle, axis);
  rotation(q);
}

void Transform::rotation(const glm::vec3& eular) {
  glm::vec3 ridians = glm::radians(eular);

  glm::mat4 RX = glm::rotate(glm::mat4(1.0), eular.x, world_up);
  glm::mat4 RY = glm::rotate(glm::mat4(1.0), eular.y, world_up);
  glm::mat4 RZ = glm::rotate(glm::mat4(1.0), eular.z, world_up);
  glm::mat4 R = RY * RZ * RX;  // Y -> Z -> X

  m_rotation = glm::quat_cast(R);
  m_transform = R * m_transform;
}

void Transform::rotation(const glm::quat& q) {
  m_rotation = m_rotation * q;
  m_transform = glm::mat4_cast(q) * m_transform;
}

}  // namespace component