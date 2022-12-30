/*
 * @Author       : xietao
 * @Date         : 2022-12-29 14:09:16
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-30 18:01:46
 * @FilePath     : /Learn_OpenGL/src/component/Material.h
 * @Description  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _SRC_COMPONENT_MATERIAL_H_
#define _SRC_COMPONENT_MATERIAL_H_

#include <memory>
#include <unordered_map>

#include "asset/Buffer.h"
#include "asset/Shader.h"
#include "asset/Texture.h"

namespace component {

class Material {
 public:
  Material(std::shared_ptr<asset::Shader> shader);

  void set_texture(GLuint u_id, std::shared_ptr<asset::Texture> texture);
  template <typename T>
  void set_uniform(const GLchar* name, const T& val) {
    m_shader->bind();
    m_shader->set_uniform(name, val);
  }
  void bind() const;
  void ubind() const;
  std::shared_ptr<asset::Shader> m_shader;
  void set_shader(std::shared_ptr<asset::Shader> shader);
 private:
  std::unordered_map<GLuint, std::shared_ptr<asset::Texture>> m_textures;
};

}  // namespace component

#endif