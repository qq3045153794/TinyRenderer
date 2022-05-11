#include "Texture2D.h" 

Texture2D::Texture2D()
  :width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), 
  wrap_s(GL_REPEAT), wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR){
  glGenBuffers(1, &this->texture);
}

void Texture2D::set_image(GLuint width, GLuint height, unsigned char* data) {
  this->width = width;
  this->height = height;
  glBindBuffer(GL_TEXTURE_2D, this->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, this->width, this->height, 0, this->image_format, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min);
  glBindBuffer(0, this->texture);
}

void Texture2D::bind() const{
  glBindBuffer(GL_TEXTURE_2D, this->texture);
}

void Texture2D::set_format(GLuint internal_format, GLuint image_format) {
  this->internal_format = internal_format;
  this->image_format = image_format;
}

void Texture2D::set_wrap(GLuint wrap_s, GLuint wrap_t) {
  this->wrap_s = wrap_s;
  this->wrap_t = wrap_t;
}

void Texture2D::set_filter(GLuint filter_min, GLuint filter_max) {
  this->filter_min = filter_min;
  this->filter_max = filter_max;
}