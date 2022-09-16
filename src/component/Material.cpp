#include "Material.h"

namespace component {

Material::Material(std::shared_ptr<asset::Shader> shader) { m_shader = shader; }

void Material::set_texture(GLuint u_id, std::shared_ptr<asset::Texture> texture) {
  m_shader->bind();
  m_shader->set_uniform(("texture_" + std::to_string(u_id)).c_str(), u_id);
  m_textures[u_id] = texture;
}

void Material::bind() const {
  m_shader->bind();
  for (const auto item : m_textures) {
    item.second->bind(item.first);
  }
}

void Material::ubind() const {
  m_shader->ubind();
  for (const auto item : m_textures) {
    item.second->ubind(item.first);
  }
}

}  // namespace component