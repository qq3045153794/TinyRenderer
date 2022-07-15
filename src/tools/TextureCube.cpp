#include "TextureCube.h"

TextureCube::TextureCube()
    : m_texture(0),
      m_internal_format({GL_RGB, GL_RGB, GL_RGB, GL_RGB, GL_RGB, GL_RGB}),
      m_image_format({GL_RGB, GL_RGB, GL_RGB, GL_RGB, GL_RGB, GL_RGB}),
      m_wrap_s(GL_REPEAT),
      m_wrap_t(GL_REPEAT),
      m_wrap_r(GL_REPEAT),
      m_filter_min(GL_LINEAR),
      m_filter_max(GL_LINEAR) {}

void TextureCube::set_image(std::vector<GLuint> width, std::vector<GLuint> height,
                            const std::vector<unsigned char*>& data) {
  m_width = width;
  m_height = height;
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

  for (size_t i = 0; i < data.size(); i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_image_format[i], m_width[i], m_height[i], 0,
                 m_internal_format[i], GL_UNSIGNED_BYTE, data[i]);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, m_filter_min);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, m_filter_max);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, m_wrap_s);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, m_wrap_t);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, m_wrap_r);
  glBindTexture(0, m_texture);
}

void TextureCube::set_wrap(GLuint warp_s, GLuint warp_t, GLuint warp_r) {
  m_wrap_s = warp_s;
  m_wrap_t = warp_t;
  m_wrap_r = warp_r;
}

void TextureCube::set_filter(GLuint filter_min, GLuint filter_max) {
  m_filter_min = filter_min;
  m_filter_max = filter_max;
}

void TextureCube::set_format(const std::vector<GLuint> image_format,
                             const std::vector<GLuint> internal_format) {
  m_image_format = image_format;
  m_internal_format = internal_format;
}

void TextureCube::Bind() const { glBindTexture(GL_TEXTURE_CUBE_MAP, GL_MAX_CUBE_MAP_TEXTURE_SIZE); }