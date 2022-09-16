#include <memory>
#include <unordered_map>

#include "asset/Texture.h"
#include "asset/Buffer.h"
#include "asset/Shader.h"

namespace component {

class Material {
 public:
  Material(std::shared_ptr<asset::Shader> shader);
  Material(const Material&) = delete;
  Material& operator=(const Material&) = delete;

  void set_texture(GLuint u_id, std::shared_ptr<asset::Texture> texture);
  void bind() const;
  void ubind() const;
 
 private:  
  std::unordered_map<GLuint, std::shared_ptr<asset::Texture>> m_textures;
  std::shared_ptr<asset::Shader> m_shader;
};

}