#include "component/Transform.h"

#include <glm/gtx/euler_angles.hpp>
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

  culate_asix();
  culate_eular();
}

void Transform::scale(const glm::vec3& s) {
  m_scale = m_scale + s;
  m_transform[0][0] = m_scale.x;
  m_transform[1][1] = m_scale.y;
  m_transform[2][2] = m_scale.z;
  m_transform[3][3] = 1.0f;

  culate_asix();
  culate_eular();
}

void Transform::rotation(const glm::vec3& axis, float angle) {
  glm::quat q = glm::angleAxis(angle, axis);
  rotation(q);

  culate_asix();
  culate_eular();
}

void Transform::rotation(const glm::vec3& eular) {
  glm::vec3 ridians = glm::radians(eular);

  glm::mat4 RX = glm::rotate(glm::mat4(1.0), eular.x, world_up);
  glm::mat4 RY = glm::rotate(glm::mat4(1.0), eular.y, world_up);
  glm::mat4 RZ = glm::rotate(glm::mat4(1.0), eular.z, world_up);
  glm::mat4 R = RY * RZ * RX;  // Y -> Z -> X

  m_rotation = glm::quat_cast(R);
  m_transform = R * m_transform;

  culate_asix();
  culate_eular();
}

void Transform::rotation(const glm::quat& q) {
  m_rotation = m_rotation * q;
  m_transform = glm::mat4_cast(q) * m_transform;
}

void Transform::culate_asix() {
  m_right = m_transform[0];
  m_up = m_transform[1];
  m_forward = -1.0f * m_transform[2];
}

void Transform::culate_eular() {
  glm::extractEulerAngleYXZ(m_transform, m_eular.y, m_eular.x, m_eular.z);
  m_eular.x = glm::degrees(m_eular.x);
  m_eular.y = glm::degrees(m_eular.y);
  m_eular.z = glm::degrees(m_eular.z);
}

const glm::mat4& Transform::get_transform() const { return m_transform; }

const glm::vec3& Transform::get_eular() const { return m_eular; }

const glm::vec3& Transform::get_position() const { return m_position; }

const glm::mat4& Transform::get_lookat() const {
  return glm::lookAt(m_position, m_position + m_forward, m_up);
}

}  // namespace component