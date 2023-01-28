#ifndef _SRC_COMPONENT_H_
#define _SRC_COMPONENT_H_

// clang-format off
#include "glm/glm.hpp"
// clang-format on

namespace component {

class Light {
 public:
  glm::vec3 m_color;
  // 光照强度
  float m_intensity;
  Light(const glm::vec3& color, float intensity);
};

class DirectionLight : public Light {
  using Light::Light;
};

class PointLight : public Light {
 public:
  float m_linear, m_quadratic;
  float m_range = std::numeric_limits<float>::max();

  using Light::Light;

  void set_attenuation(float linear, float quadratic);
  float get_attenuation(float distance) const;
};

class Spotlight : public Light {
 public:
  float m_inner_cutoff;
  float m_outer_cutoff;
  float m_range = std::numeric_limits<float>::max();
  using Light::Light;

  void set_cutoff(float range, float inner_cutoff = 15.0f, float outer_cutoff = 30.0f);
  float get_inner_cosine() const;
  float get_outer_cosine() const;
  float get_attenuation(float distance) const;
};

}  // namespace component

#endif
