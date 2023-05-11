#include "Material.h"

#include <library/ShaderLibrary.h>

#include "core/App.h"
#include "core/Debug.h"
#include "library/TextureLibrary.h"
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
    {200, "is_animation"},
    {997, "irradiance_map"}
};

std::map<GLuint, std::string> Material::texture_dictionary{
    {0, "texture_0"},
    {1, "texture_1"},
    {2, "irradiance_map"},
    {3, "prefilter_map"},
    {4, "brdf_LUT_map"},
    {5, "albedo_texture"},
    {6, "metalness_texture"},
    {7, "roughness_texture"},
    {8, "ao_texture"},
    {9, "normal_texture"},
    {13, "shadow_map"}
};
// clang-format on

Material::Material(std::shared_ptr<asset::Shader> shader) : m_shading_model(ShadingModel::COSTEM) {
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

Material::Material(ShadingModel shadering_model) : m_shading_model(shadering_model) {
  if (shadering_model == ShadingModel::DEFAULT) {
    auto default_shader = ::saber::PublicSingleton<saber::Library<::asset::Shader>>::GetInstance().GetDefaultShader();
    set_shader(default_shader);
    // 初始化未有动画
    set_uniform(200U, false);

  } else if (shadering_model == ShadingModel::PBR) {
    auto pbr_shader = ::saber::PublicSingleton<saber::Library<::asset::Shader>>::GetInstance().GetPbrShader();
    set_shader(pbr_shader);
    set_uniform(100U, false);
    set_uniform(101U, false);
    set_uniform(102U, false);
    set_uniform(103U, false);
    set_uniform(104U, false);

    set_uniform(200U, false);

    // 标准模型
    set_uniform(0U, glm::vec4(1.0f));
    set_uniform(1U, 0.0f);
    set_uniform(2U, 1.0f);
    set_uniform(3U, 1.0f);
    set_uniform(4U, 0.5f);


    auto shadow_map = ::saber::PublicSingleton<::saber::Library<::asset::Texture>>::GetInstance().Get("shadow_map");
    set_texture(13U, shadow_map);
  } else if (shadering_model == ShadingModel::DEFAULT) {
  }
}

void Material::set_shader(std::shared_ptr<asset::Shader> shader) { m_shader = shader; }

void Material::bind_texture(pbr_t pbr, std::shared_ptr<asset::Texture> texture) {
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

    m_uniforms_cache[texture_dictionary[uid]] = UboData<GLuint>(texture_dictionary[uid], m_shader, uid);
  } else {
    CORE_ERROR("Can't find valid texture units (uid = {})", uid);
  }
}


Material::ubo_variant Material::get_uniform(pbr_u id) {
  std::string uniform_key = uniform_dictionary[(uint16_t)id];
  CORE_ASERT(m_uniforms_cache.count(uniform_key) > 0, "No find uniform (id = {})", (uint16_t)id);
  return m_uniforms_cache[uniform_key];
}


Material::ubo_variant Material::get_uniform(const std::string& key) {
  CORE_ASERT(m_uniforms_cache.count(key) > 0, "No find uniform (key = {})", key);
  return m_uniforms_cache[key];
}


std::shared_ptr<::asset::Texture> Material::get_texture(pbr_t pbr) {
  auto texture_key = static_cast<GLuint>(pbr);
  CORE_ASERT(m_textures.count(static_cast<GLuint>(pbr)) > 0, "No find Textures (key = {})", texture_key);
  return m_textures[texture_key];
}


std::shared_ptr<::asset::Texture> Material::get_texture(GLuint uid) {

  CORE_ASERT(m_textures.count(static_cast<GLuint>(uid)) > 0, "No find Textures (key = {})", uid);
  return m_textures[uid];
}

void Material::bind() const {
  m_shader->bind();
  for (const auto& item : m_textures) {
    item.second->bind(item.first);
  }

  for (auto& [i, uniform] : m_uniforms_cache) {
    std::visit([](auto& val) { val.bind(); }, uniform);
  }
}

void Material::ubind() const {
  m_shader->ubind();
  for (const auto& item : m_textures) {
    item.second->ubind(item.first);
  }
}

}  // namespace component
