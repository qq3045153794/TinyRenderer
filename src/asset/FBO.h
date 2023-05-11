#ifndef _SRC_ASSERT_FBO_H_
#define _SRC_ASSERT_FBO_H_

#include "asset/Asset.h"
#include "asset/Buffer.h"
#include "asset/Shader.h"
#include "asset/Texture.h"
#include "asset/VAO.h"

namespace asset {
class RBO : public Asset {
 public:
  RBO(GLuint width, GLuint height);
  virtual ~RBO();
  void bind() const override;
  void ubind() const override;
};

class FBO : public Asset {
 public:
  FBO(GLuint width, GLuint height);
  virtual ~FBO();
  void set_color_texture();
  void set_color_texture(GLuint index, GLuint cubemap, GLuint face) const;
  void set_depth_texture();
  void reset_depth_texture(GLuint width, GLuint height);
  void change_shader(std::unique_ptr<Shader> shader);
  const std::unique_ptr<Shader>& get_shader() const;
  const Texture& get_color_texture();
  void draw() const;
  void draw_ex() const;
  void bind() const override;
  void ubind() const override;
  GLuint Width() const;
  GLuint Height() const;

  void render_pass_texture(std::shared_ptr<Texture> texture);

 private:
  void set_buffer();
  std::unique_ptr<Shader> m_shader;
  std::unique_ptr<VAO> m_vao;
  std::unique_ptr<VBO> m_vbo;
  std::unique_ptr<IBO> m_ibo;

  std::unique_ptr<Texture> m_texture;
  std::unique_ptr<RBO> m_rbo;
  GLuint m_width, m_height;
};

}  // namespace asset

#endif
