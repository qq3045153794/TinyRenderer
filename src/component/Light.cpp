#include "component/Light.h"

#include <algorithm>

#include "core/Log.h"

namespace component {

Light::Light(const glm::vec3& color, float intensity) : m_color(color), m_intensity(intensity) {}

void PointLight::set_attenuation(float linear, float quadratic) {
  CORE_ASERT(linear > 0, "The linear attenuation factor must be positive ...");
  CORE_ASERT(quadratic > 0, "The quadratic attenuation factor must be positive ...");

  m_linear = linear;
  m_quadratic = quadratic;
  // 距离范围限制
  m_range = 1e9;
}

float PointLight::get_attenuation(float distance) const {
  // 衰减与距离成反比
  // 引用 https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
  CORE_ASERT(distance >= 0.0f, "Distance to the light source cannot be negative ...");

  return distance > 1e9 ? 0 : 1 / (1.0 + m_linear * distance + m_quadratic * distance * distance);
}

void Spotlight::set_cutoff(float range, float inner_cutoff, float outer_cutoff) {
  CORE_ASERT(range > 0, "The spotlight range must be positive ...");
  CORE_ASERT(inner_cutoff > 0, "The inner cutoff angle must be positive ...");
  CORE_ASERT(outer_cutoff > 0, "The outer cutoff angle must be positive ...");

  m_inner_cutoff = inner_cutoff;
  m_outer_cutoff = outer_cutoff;
  m_range = range;
}

float Spotlight::get_inner_cosine() const { return glm::cos(glm::radians(m_inner_cutoff)); }

float Spotlight::get_outer_cosine() const { return glm::cos(glm::radians(m_outer_cutoff)); }
float Spotlight::get_attenuation(float distance) const {
  CORE_ASERT(distance >= 0.0f, "Distance to the light source cannot be negative ...");

  return 1.0 - std::clamp(distance / m_range, 0.0f, 1.0f);
}

}  // namespace component