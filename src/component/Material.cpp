#include "Material.h"

#include "core/App.h"
namespace component {

// clang-format off
std::map<GLuint, std::string> Material::uniform_dictionary{
    {0, "albedo"},
    {1, "metalness"},
    {2, "roughness"},
    {3, "ao"},
    {4, "specular"},
    {100, "sample_albedo"},
    {101, "sample_metalness"},
    {102, "sample_roughness"},
    {103, "sample_ao"},
    {104, "sample_normal"}
};

std::map<GLuint, std::string> Material::texture_dictionary{
    {0, "texture_0"},
    {1, "texture_1"},
    {2, "texture_2"},
    {3, "texture_3"},
    {4, "texture_4"},
    {5, "albedo_texture"},
    {6, "metalness_texture"},
    {7, "roughness_texture"},
    {8, "ao_texture"},
    {9, "normal_texture"}
};
// clang-format on

Material::Material(std::shared_ptr<asset::Shader> shader) {
  set_shader(shader);
  if (m_shader != nullptr) {
    set_uniform(100U, false);
    set_uniform(101U, false);
    set_uniform(102U, false);
    set_uniform(103U, false);
    set_uniform(104U, false);

    // 标准模型
    set_uniform(0U, glm::vec4(1.0f));
    set_uniform(1U, 0.0f);
    set_uniform(2U, 1.0f);
    set_uniform(3U, 1.0f);
    set_uniform(4U, 0.5f);
  }
}

void Material::set_shader(std::shared_ptr<asset::Shader> shader) {
  m_shader = shader;

  GLuint id = m_shader->get_id();
  GLint n_uniforms;
}

void Material::set_texture(GLuint uid, std::shared_ptr<asset::Texture> texture) {
  m_shader->bind();

  int n_texture = m_textures.size();
  int max_saplers = core::App::instand().gl_max_texture_units;

  if (n_texture > max_saplers) {
    CORE_ERROR("{0} samplers limit has reached, failed to add texture", max_saplers);
    return;
  }

  if (texture_dictionary.count(uid) > 0) {
    m_shader->set_uniform(texture_dictionary[uid].c_str(), uid);
    m_textures.insert_or_assign(uid, texture);
  } else {
    CORE_ERROR("Can't find valid texture units (uid = {})", uid);
  }
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