#include "TexCubeRender.h"

TexCubeRender::TexCubeRender()
    : m_vao(),
      m_vbo(),
      m_shader(ResourceManager::load_shader("../resource/shader/TexCube.vs",
                                            "../resource/shader/TexCube.fs", "TexCubeShader")),
      m_tex_cube(ResourceManager::load_texture_cube(
          {"../resource/texture/skybox/right.jpg", "../resource/texture/skybox/left.jpg",
           "../resource/texture/skybox/top.jpg", "../resource/texture/skybox/bottom.jpg",
           "../resource/texture/skybox/front.jpg", "../resource/texture/skybox/back.jpg"},
          "sky_box")) {
  setup_buffers();
}

TexCubeRender::~TexCubeRender() {}

void TexCubeRender::setup_buffers() {
  float data[] = {// positions
                  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                  -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
}

void TexCubeRender::render(gl_cameras::Camera& c) {
  // 不写入深度buffer
  glDepthMask(GL_FALSE);

  m_shader.use();
  glm::mat4 projection = c.get_projection_mat();
  m_shader.set_matrix4("projection", projection);
  glm::mat4 view = glm::mat4(glm::mat3(c.get_view_mat()));
  m_shader.set_matrix4("view", view);
  glm::mat4 model(1.0);
  m_shader.set_matrix4("model", model);

  glActiveTexture(GL_TEXTURE0);
  m_tex_cube.Bind();
  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  glDepthMask(GL_TRUE);
}

const TextureCube& TexCubeRender::get_tex_cube() const { return m_tex_cube; }