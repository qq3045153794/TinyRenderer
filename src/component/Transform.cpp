#include "component/Transform.h"

#include <glm/gtx/euler_angles.hpp>

#include "iostream"
namespace component {

Transform::Transform()
    : m_transform(1.0),
      m_position(0.0),
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
  m_scale = m_scale * s;
  m_transform = glm::scale(m_transform, s);

  culate_asix();
  culate_eular();
}

void Transform::rotation(const glm::vec3& axis, float angle) {
  float radians = glm::radians(angle);
  glm::quat q = glm::normalize(glm::angleAxis(radians, axis));
  rotation(q);

  culate_asix();
  culate_eular();
}

void Transform::rotation(const glm::vec3& eular) {
  glm::vec3 ridians = glm::radians(eular);

  glm::mat4 RX = glm::rotate(glm::mat4(1.0), eular.x, world_right);
  glm::mat4 RY = glm::rotate(glm::mat4(1.0), eular.y, world_up);
  glm::mat4 RZ = glm::rotate(glm::mat4(1.0), eular.z, -world_forward);

  glm::mat4 R = glm::eulerAngleYZX(eular.y, eular.z, eular.x);
  // glm::mat4 R = RY * RZ * RX;  // Y -> Z -> X

  m_rotation = glm::normalize(glm::quat_cast(R) * m_rotation);
  m_transform = R * m_transform;
  m_position = m_transform[3];

  culate_asix();
  culate_eular();
}

void Transform::rotation(const glm::quat& q) {
  m_rotation = glm::normalize(glm::normalize(q) * m_rotation);
  m_transform = glm::mat4_cast(glm::normalize(q)) * m_transform;
  m_position = m_transform[3];

  culate_asix();
  culate_eular();
}

void Transform::culate_asix() {
  m_right = glm::normalize(m_transform[0]);
  m_up = glm::normalize(m_transform[1]);
  m_forward = glm::normalize(-1.f * m_transform[2]);
}

void Transform::culate_eular() {
  glm::extractEulerAngleYZX(m_transform, m_eular.y, m_eular.z, m_eular.x);
  m_eular.x = glm::degrees(m_eular.x);
  m_eular.y = glm::degrees(m_eular.y);
  m_eular.z = glm::degrees(m_eular.z);
}

const glm::mat4& Transform::get_transform() const { return m_transform; }

glm::vec3& Transform::get_eular() { return m_eular; }

glm::vec3& Transform::get_position() { return m_position; }

glm::vec3& Transform::get_scale() { return m_scale; }

const glm::mat4 Transform::get_lookat() const {
  return glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Transform::set_ealar_YZX(const glm::vec3& eular) {
  glm::vec3 temp_eular;
  temp_eular.x = glm::radians(eular.x);
  temp_eular.y = glm::radians(eular.y);
  temp_eular.z = glm::radians(eular.z);

  auto& temp_pos = m_transform[3];
  std::cout << "temp_pos : " << temp_pos.x << " " << temp_pos.y << " " << temp_pos.z << std::endl;

  m_transform = glm::mat4(1.0);
  m_transform[0][0] = m_scale.x;
  m_transform[1][1] = m_scale.y;
  m_transform[2][2] = m_scale.z;
  m_transform[3][3] = 1.0;

  m_rotation = glm::normalize(glm::quat_cast(
      glm::eulerAngleYZX(temp_eular.y, temp_eular.z, temp_eular.x)));
  m_transform = glm::mat4_cast(m_rotation) * m_transform;

  m_transform[3] = temp_pos;
  culate_asix();
  culate_eular();
}

void Transform::set_rotation(const glm::quat& q) {
  auto& temp_pos = m_transform[3];

  m_transform = glm::mat4(1.0);
  m_transform[0][0] = m_scale.x;
  m_transform[1][1] = m_scale.y;
  m_transform[2][2] = m_scale.z;
  m_transform[3][3] = 1.0;

  m_rotation = glm::normalize(glm::normalize(q) * m_rotation);
  m_transform = glm::mat4_cast(m_rotation) * m_transform;

  m_transform[3] = temp_pos;

  culate_asix();
  culate_eular();
}

void Transform::set_position(const glm::vec3& position) {
  m_position = position;
  m_transform[3] = glm::vec4(position, 1.0);

  culate_asix();
  culate_eular();
}

void Transform::set_scale(const glm::vec3& size) {
  scale(size / m_scale);
  culate_asix();
  culate_eular();
}

void Transform::set_transform(const glm::mat4& transform) {
  m_scale.x = glm::length(transform[0]);
  m_scale.y = glm::length(transform[1]);
  m_scale.z = glm::length(transform[2]);

  glm::mat3 pure_rotation_mat = {transform[0] / m_scale.x,
                                 transform[1] / m_scale.y,
                                 transform[2] / m_scale.z};

  m_transform = transform;
  m_position = transform[3];
  m_rotation = glm::normalize(glm::quat_cast(pure_rotation_mat));

  culate_asix();
  culate_eular();
}

}  // namespace component
