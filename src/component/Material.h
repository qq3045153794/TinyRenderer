#ifndef _LEARN_OPENGL_SRC_COMPONENT_MATERIAL_H_
#define _LEARN_OPENGL_SRC_COMPONENT_MATERIAL_H_

#include <memory>
#include <unordered_map>

#include "asset/Buffer.h"
#include "asset/Shader.h"
#include "asset/Texture.h"

namespace component {

class Material {
 public:
  Material(std::shared_ptr<asset::Shader> shader);
  Material(const Material&) = delete;
  Material& operator=(const Material&) = delete;

  void set_texture(GLuint u_id, std::shared_ptr<asset::Texture> texture);
  template <typename T>
  void set_uniform(const GLchar* name, const T& val) {
    m_shader->bind();
    m_shader->set_uniform(name, val);
  }
  void bind() const;
  void ubind() const;

 private:
  std::unordered_map<GLuint, std::shared_ptr<asset::Texture>> m_textures;
  std::shared_ptr<asset::Shader> m_shader;
};

}  // namespace component

#endif