#include "YUVFrame.h"

namespace gl_frame {

YUVFrame::YUVFrame(GLuint width, GLuint height, GLuint sampler)
    : m_width(width),
      m_height(height),
      m_sampler(sampler),
      m_process(NONE),
      m_texture(),
      m_mult_texture(),
      m_shader(ResourceManager::load_shader("../resource/shader/frame.vs",
                                            "../resource/shader/frame.fs", "yuv_shader")) {
  setup_frame_buffer();
  setup_screem_buffer();
  // 设置后期处理
  set_process(m_process);
}

YUVFrame::~YUVFrame() {
  glDeleteVertexArrays(1, &m_vao);
  glDeleteFramebuffers(1, &m_fbo);
  glDeleteFramebuffers(1, &m_mult_fbo);
  glDeleteRenderbuffers(1, &m_mult_rbo);
  glDeleteBuffers(1, &m_vbo);
}

void YUVFrame::set_process(Process process) {
  m_process = process;
  m_shader.set_integer("process", static_cast<int>(m_process), true);
}

void YUVFrame::setup_frame_buffer() {
  glGenFramebuffers(1, &m_mult_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_mult_fbo);

  m_mult_texture.set_mult_image(m_width, m_height, m_sampler);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
                         m_mult_texture.get_texture_id(), 0);

  // 创建个渲染缓冲 将其作为深度缓冲添加到帧缓冲中
  glGenRenderbuffers(1, &m_mult_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, m_mult_rbo);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_sampler, GL_DEPTH24_STENCIL8, m_width,
                                   m_height);
  glBindRenderbuffer(0, m_mult_rbo);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                            m_mult_rbo);
  // 检查帧缓冲
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! 1" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  // 创建个纹理 将其作为颜色附加添加到帧缓冲中
  m_texture.set_image(m_width, m_height, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         m_texture.get_texture_id(), 0);

  // 检查帧缓冲
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! 2" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void YUVFrame::begin_render() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_mult_fbo);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
}

void YUVFrame::end_render() {
  // 读取MSAA FBO
  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_mult_fbo);
  // 写入普通 FBO
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

  // 将源的颜色缓冲输入到目标的颜色缓冲中
  glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT,
                    GL_NEAREST);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
}

void YUVFrame::setup_screem_buffer() {
  GLfloat data[6][5] = {{-1.0, -1.0, 0.0, 0.0, 0.0}, {-1.0, 1.0, 0.0, 0.0, 1.0},
                        {1.0, -1.0, 0.0, 1.0, 0.0},  {1.0, -1.0, 0.0, 1.0, 0.0},
                        {-1.0, 1.0, 0.0, 0.0, 1.0},  {1.0, 1.0, 0.0, 1.0, 1.0}};

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                        (void*)(sizeof(GLfloat) * 3));
  glEnableVertexAttribArray(1);
}

void YUVFrame::render() {
  m_shader.use();
  m_shader.set_integer("texture_0", 0);
  glActiveTexture(GL_TEXTURE0);
  m_texture.bind();

  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

}  // namespace gl_frame