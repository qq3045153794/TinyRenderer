#include "FBO.h"

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
}

FBO::~FBO() {
  glDeleteFramebuffers(1, &id);
}

void FBO::set_color_texture() {
  m_texture = std::make_unique<Texture> (m_width, m_height);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture->get_id(), 0);
}

void FBO::set_depth_texture() {
  m_rbo = std::make_unique<RBO> (m_width, m_height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo->get_id());
}

const Texture& FBO::get_color_texture() {
  return *m_texture;
}

}