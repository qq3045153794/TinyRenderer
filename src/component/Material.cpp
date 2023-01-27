#include "Material.h"

#include "core/App.h"
#include "core/Debug.h"
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
    {104, "sample_normal"},
    {997, "irradiance_map"}
};

std::map<GLuint, std::string> Material::texture_dictionary{
    {0, "texture_0"},
    {1, "texture_1"},
    {2, "irradiance_map"},
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
}


void Material::set_texture(pbr_t pbr, std::shared_ptr<asset::Texture> texture) {
  GLuint uid = static_cast<GLuint>(pbr);
  if (texture_dictionary.find(uid) != texture_dictionary.end()) {
    std::string texture_name = texture_dictionary[uid];
    if (uid >= 5U) {
      set_uniform(uid + 95U, true);
    }
    set_texture(uid, texture);
  } else {
    CORE_ERROR("Can't find valid texture (uid = {0})", uid);
  }
}

void Material::set_texture(GLuint uid, std::shared_ptr<asset::Texture> texture) {

CHECK_ERROR();
  m_shader->bind();

CHECK_ERROR();
  int n_texture = m_textures.size();
  int max_saplers = core::App::instand().gl_max_texture_units;

  if (n_texture > max_saplers) {
    CORE_ERROR("{0} samplers limit has reached, failed to add texture", max_saplers);
    return;
  }

CHECK_ERROR();
  if (texture_dictionary.count(uid) > 0) {

CHECK_ERROR();
    m_shader->set_uniform(texture_dictionary[uid].c_str(), uid);
CHECK_ERROR();
    m_textures.insert_or_assign(uid, texture);
CHECK_ERROR();
  } else {
    CORE_ERROR("Can't find valid texture units (uid = {})", uid);
  }
}

void Material::bind() const {
  m_shader->bind();
  for (const auto& item : m_textures) {
    item.second->bind(item.first);
  }
}

void Material::ubind() const {
  m_shader->ubind();
  for (const auto& item : m_textures) {
    item.second->ubind(item.first);
  }
}

}  // namespace component
