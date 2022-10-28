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
  m_shader = std::make_unique<Shader> ("../resource/shader/fbo.vs","../resource/shader/fbo.fs");
  m_shader->bind();
  m_shader->set_uniform("texture_0", 0);
  set_buffer();
}

FBO::~FBO() {
  glDeleteFramebuffers(1, &id);
}

void FBO::set_buffer() {
  GLfloat data [] = {
    -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  0.0f, 1.0f, 0.0f,
     1.0f, -1.0f,  0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f, 1.0f
  };

  GLuint index [] = {
    0, 1, 2, 
    0, 3, 2
  };
  
  m_vbo = std::make_unique<VBO> (sizeof(data), data, GL_STATIC_DRAW);
  m_ibo = std::make_unique<IBO> (sizeof(index), index, GL_STATIC_DRAW, 6U);
  m_vao = std::make_unique<VAO> ();
  
  m_vao->set_vbo(*m_vbo, 0, 3U, sizeof(GLfloat) * 5, 0U, GL_FLOAT);
  m_vao->set_vbo(*m_vbo, 1, 2U, sizeof(GLfloat) * 5, 3U * sizeof(GLfloat), GL_FLOAT);
  m_vao->set_ibo(*m_ibo);
}

void FBO::set_color_texture() {
  this->bind();
  m_texture = std::make_unique<Texture> (m_width, m_height);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->get_id(), 0);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  this->ubind();
}

void FBO::set_depth_texture() {
  this->bind();
  m_rbo = std::make_unique<RBO> (m_width, m_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo->get_id());

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  this->ubind();
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