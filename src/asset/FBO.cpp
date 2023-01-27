#include "FBO.h"
#include "core/Log.h"

namespace asset {

RBO::RBO(GLuint width, GLuint height) {
  glGenRenderbuffers(1, &id);
  glBindRenderbuffer(GL_RENDERBUFFER, id);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
}

RBO::~RBO() {
  glDeleteRenderbuffers(1, &id);
}

void RBO::bind() const {
  glBindRenderbuffer(GL_RENDERBUFFER, id);
}

void RBO::ubind() const {
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FBO::FBO(GLuint width, GLuint height) {
  glGenFramebuffers(1, &id);
  m_width = width;
  m_height = height;
  m_shader = std::make_unique<Shader> ("../resource/shader/fbo.vs",  "../resource/shader/fbo.fs");
  m_shader->bind();
  m_shader->set_uniform("texture_0", 0);
  set_buffer();
}

FBO::~FBO() {
  glDeleteFramebuffers(1, &id);
}

void FBO::set_buffer() {
  // clang-format off
  GLfloat data [] = {
    -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  // left top
     1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  // right top
     1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  // right bottom
    -1.0f, -1.0f,  0.0f, 0.0f, 0.0f   // left bottom
  };

  GLuint index [] = {
    0, 2, 1,  // CCW
    0, 3, 2   // CCW 
  };
  // clang-format on

  m_vbo = std::make_unique<VBO> (sizeof(data), data, GL_STATIC_DRAW);
  m_ibo = std::make_unique<IBO> (sizeof(index), index, GL_STATIC_DRAW, 6U);
  m_vao = std::make_unique<VAO> ();

  m_vao->set_vbo(*m_vbo, 0, 3U, sizeof(GLfloat) * 5, 0U, GL_FLOAT);
  m_vao->set_vbo(*m_vbo, 1, 2U, sizeof(GLfloat) * 5, 3U * sizeof(GLfloat), GL_FLOAT);
  m_vao->set_ibo(*m_ibo);
}

void FBO::set_color_texture() {
  this->bind();
  m_texture = std::make_unique<Texture> (GL_TEXTURE_2D, m_width, m_height, GL_RGB);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->get_id(), 0);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }
  // this->ubind();
  CORE_INFO("attach color texture");
}


void FBO::set_color_texture(GLuint index, GLuint cubemap, GLuint face) {
  this->bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, cubemap, 0);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }
  // this->ubind(); 
  CORE_INFO("attach color cubemap texture (face = {})", face);
}

void FBO::set_depth_texture() {
  this->bind();
  m_rbo = std::make_unique<RBO> (m_width, m_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo->get_id());

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }
  // this->ubind();
  CORE_INFO("attach depth texture");
}


void FBO::reset_depth_texture(GLuint width, GLuint height) {
  if (m_rbo) {
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo->get_id());
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  } else {
    CORE_ERROR("Reset depth texture failed, FBO not Attach RBO...");
  }
}

void FBO::change_shader(std::unique_ptr<Shader> shader) {
  m_shader = std::move(shader);
}

const std::unique_ptr<Shader>& FBO::get_shader() const {
  return m_shader;
}

void FBO::draw() const{
  m_texture->bind(0);
  m_shader->bind();
  m_vao->draw();
  m_texture->ubind(0);
}

const Texture& FBO::get_color_texture() {
  return *m_texture;
}

void FBO::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FBO::ubind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
