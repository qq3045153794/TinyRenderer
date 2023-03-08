/*
 * @Author       : xietao
 * @Date         : 2022-12-29 14:09:16
 * @LastEditors  : xietao
 * @LastEditTime : 2023-01-06 16:30:24
 * @FilePath     : TinyRenderer/src/component/Material.h
 * @Description  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _SRC_COMPONENT_MATERIAL_H_
#define _SRC_COMPONENT_MATERIAL_H_

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "asset/Buffer.h"
#include "asset/Shader.h"
#include "asset/Texture.h"
#include "core/Log.h"

namespace component {

class Material {
 public:
  // 引用 : https://zhuanlan.zhihu.com/p/260973533
  enum class pbr_u : uint16_t {
    albedo = 0U,     // 反射率 向量 sRGB 主要体现纹理和颜色
    metalness = 1U,  // 金属度 标量 [0..1]
    roughness = 2U,  // 粗糙度 标量 [0..1]
    ao = 3U,         // 反射率 标量 [0..1]
    specular = 4U
  };

  // 纹理采样数据
  enum class pbr_t : uint16_t {
    albedo = 5U,     // 反射率 向量 sRGB 主要体现纹理和颜色
    metalness = 6U,  // 金属度 标量 [0..1]
    roughness = 7U,  // 粗糙度 标量 [0..1]
    ao = 8U,         // 反射率 标量 [0..1]
    normal = 9U,     // 法线贴图 向量 切线空间
    irradiance_map = 2U,
    prefilter_map = 3U,
    brdf_LUT_map = 4U
  };

  static std::map<GLuint, std::string> uniform_dictionary;
  static std::map<GLuint, std::string> texture_dictionary;

  Material(std::shared_ptr<asset::Shader> shader);
  void set_texture(GLuint u_id, std::shared_ptr<asset::Texture> texture);
  void set_texture(pbr_t pbr, std::shared_ptr<asset::Texture> texture);
  void bind() const;
  void ubind() const;
  std::shared_ptr<asset::Shader> m_shader;
  void set_shader(std::shared_ptr<asset::Shader> shader);

  template <typename T>
  void set_uniform(const GLchar* name, const T& val) {
    m_shader->bind();
    m_shader->set_uniform(name, val);
  }

  template <typename T>
  void set_uniform(GLuint uid, const T& val) {
    m_shader->bind();
    if (uniform_dictionary.find(uid) != uniform_dictionary.end()) {
      m_shader->set_uniform(uniform_dictionary[uid].c_str(), val);
    } else {
      CORE_ERROR("Can't find valid uniform (uid = {})", uid);
    }
  }

  template <typename T>
  void bing_uniform(pbr_u pbr, const T& val) {
    GLuint uid = static_cast<GLuint>(pbr);
    set_uniform(uid, val);
  }

 private:
  std::unordered_map<GLuint, std::shared_ptr<asset::Texture>> m_textures;
};

}  // namespace component

#endif
