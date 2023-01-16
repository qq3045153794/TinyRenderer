#include "asset/Texture.h" 
#include <iostream>
#include <memory>
#include "stb_image.h"
#include "utils/image.h"
#include "asset/FBO.h"
namespace asset {

Texture::Texture(const GLchar* img_path, bool flip) : m_target(GL_TEXTURE_2D) {
  const auto& image = utils::Image(img_path, flip);
  m_width = image.get_width();
  m_height = image.get_height();
  m_image_format = image.get_img_format();
  m_internal_format = image.get_ine_format();
  glGenTextures(1, &m_id);
  glBindTexture(m_target, m_id);
  glTexImage2D(m_target, 0, m_internal_format, m_width, m_height, 0, m_image_format,
               GL_UNSIGNED_BYTE, image.get_buffer());
  glBindTexture(m_target, 0);

  set_sampler_state();
}

Texture::Texture(const std::vector<std::string>& path_vec) : m_target(GL_TEXTURE_CUBE_MAP) {
  glGenTextures(1, &m_id);
  glBindTexture(m_target, m_id);

  int idx = 0;
  for (const auto& img_path : path_vec) {
    const auto& image = utils::Image(img_path);
    m_width = image.get_width();
    m_height = image.get_height();
    m_image_format = image.get_img_format();
    m_internal_format = image.get_ine_format();

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, m_internal_format, m_width, m_height, 0,
                 m_image_format, GL_UNSIGNED_BYTE, image.get_buffer());
    ++idx;
  }
  glBindTexture(m_target, 0);
  set_sampler_state();
}

Texture::Texture(const GLchar* path, int resolution) : m_target(GL_TEXTURE_CUBE_MAP){


  const auto& image = utils::Image(path, true);
  m_width = image.get_width();
  m_height = image.get_height();
  m_image_format = image.get_img_format();
  m_internal_format = image.get_ine_format();
  // 创建2D texture
  glGenTextures(1, &m_id);
  glBindTexture(GL_TEXTURE_2D, m_id);
  glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format, m_width, m_height, 0, m_image_format,
               GL_FLOAT, image.get_buffer());
  glBindTexture(GL_TEXTURE_2D, 0);

  std::unique_ptr<FBO> cubemap_fbo = std::make_unique<FBO>(resolution, resolution);
  
  cubemap_fbo->set_depth_texture();

  for (size_t i = 0; i < 6; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                 resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
  }

  set_sampler_state();


}
Texture::Texture(GLuint width, GLuint height) : m_target(GL_TEXTURE_2D) {
  m_width = width;
  m_height = height;
  m_image_format = GL_RGB;
  m_internal_format = GL_RGB;
  glGenTextures(1, &m_id);
  glBindTexture(m_target, m_id);
  glTexImage2D(m_target, 0, m_internal_format, m_width, m_height, 0, m_image_format,
               GL_UNSIGNED_BYTE, NULL);
  glBindTexture(m_target, 0);

  set_sampler_state();
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

GLuint Texture::get_id() { return m_id; }

void Texture::set_sampler_state() {
  glBindTexture(m_target, m_id);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (m_target == GL_TEXTURE_CUBE_MAP) {
    glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
  }

  glBindTexture(m_target, 0);
}

void Texture::bind(GLuint unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(m_target, m_id);
}

void Texture::ubind(GLuint unit) const { glBindTexture(m_target, 0); }

}  // namespace asset
