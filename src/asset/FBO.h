#include "asset/Asset.h"
#include "asset/Texture.h"
#include "asset/Shader.h"
#include "asset/VAO.h"

namespace asset {
class RBO : public Asset{
 public:
  RBO(GLuint width, GLuint height);
  virtual ~RBO();
  void bind() const override ;
  void ubind() const  override;
};

class FBO : public Asset {
 public:
  FBO(GLuint width, GLuint height);
  virtual ~FBO();
  void set_color_texture();
  void set_depth_texture();
  const Texture& get_color_texture();
 private:

  std::unique_ptr<Texture> m_texture;
  std::unique_ptr<RBO> m_rbo;
  GLuint m_width, m_height;
};

}