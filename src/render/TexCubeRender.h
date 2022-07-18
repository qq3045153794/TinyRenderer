#ifndef _LEARN_OPENGL_SRC_RENDER_TEXCUBERENDER_H_
#define _LEARN_OPENGL_SRC_RENDER_TEXCUBERENDER_H_

#include "Camera.h"
#include "ResourceManager.h"

class TexCubeRender {
 public:
  TexCubeRender();
  TexCubeRender(const TexCubeRender&) = delete;
  TexCubeRender& operator=(const TexCubeRender&) = delete;
  ~TexCubeRender();
  void render(gl_cameras::Camera& c);
  const TextureCube& get_tex_cube() const;
 private:
  void setup_buffers();
  GLuint m_vao;
  GLuint m_vbo;
  Shader m_shader;
  TextureCube m_tex_cube;
};

#endif