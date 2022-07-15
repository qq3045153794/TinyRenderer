#include "YUVFrame.h"

namespace gl_frame {

YUVFrame::YUVFrame(GLuint width, GLuint height)
    : m_width(width),
      m_height(height),
      m_texture(),
      m_shader(ResourceManager::load_shader("../resource/shader/frame.vs",
                                            "../resource/shader/frame.fs", "yuv_shader")) {
  setup_frame_buffer();
  setup_screem_buffer();
}

YUVFrame::~YUVFrame() {
  glDeleteVertexArrays(1, &m_vao);
  glDeleteFramebuffers(1, &m_fbo);

  glDeleteBuffers(1, &m_vbo);
}

void YUVFrame::setup_frame_buffer() {
  glGenFramebuffers(1, &m_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

  // 创建个纹理 将其作为颜色附加添加到帧缓冲中
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

  // 创建个渲染缓冲 将其作为深度缓冲添加到帧缓冲中
  glGenRenderbuffers(1, &m_rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
  glBindRenderbuffer(0, m_rbo);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

  // 检查帧缓冲
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void YUVFrame::begin_render() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
  glEnable(GL_DEPTH_TEST);


  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void YUVFrame::end_render() {


  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
  // clear all relevant buffers
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
  glClear(GL_COLOR_BUFFER_BIT);
}

void YUVFrame::setup_screem_buffer() {
  //GLfloat data[6][5] = {{-1.0, -1.0, 0.0, 0.0, 1.0}, {-1.0, 1.0, 0.0, 0.0, 0.0},
  //                      {1.0, -1.0, 0.0, 1.0, 1.0},  {1.0, -1.0, 0.0, 1.0, 1.0},
  //                      {-1.0, 1.0, 0.0, 0.0, 0.0},  {1.0, 1.0, 0.0, 1.0, 0.0}};

  float data[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f, 0.0,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0,  1.0f, 0.0f,

        -1.0f,  1.0f, 0.0,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0,  1.0f, 1.0f
    };

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
  glGenVertexArrays(0, &m_vao);
}

void YUVFrame::render() {
  m_shader.use();
  m_shader.set_integer("texture_0", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glBindVertexArray(m_vao);  

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

}  // namespace gl_frame