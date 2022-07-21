#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "utils/image.h"

#include "stb_image.h"
#include "glad/glad.h"

namespace utils {
Image::Image(const std::string& img_path, bool flip) : m_width(0), m_height(0), n_channels(0) {
  stbi_set_flip_vertically_on_load(flip);
  m_buffer = stbi_load(img_path.c_str(), &m_width, &m_height, &n_channels, 0);

  if(!m_buffer){
    std::cout << "Texture failed to load at path: " << img_path << std::endl;
  }
}

Image::~Image() {
  stbi_image_free(m_buffer);
  
}

GLuint Image::get_width() const {
  return static_cast<GLuint> (m_width);
}

GLuint Image::get_height() const {
  return static_cast<GLuint> (m_height);
}

GLuint Image::get_img_format() const {
  switch(n_channels) {
    case 1 : return GL_RED;
    case 2 : return GL_RG;
    case 3 : return GL_RGB;
    case 4 : return GL_RGBA;
    default: return 0;
  }
}

GLuint Image::get_ine_format() const {
  switch(n_channels) {
    case 1 : return GL_RED;
    case 2 : return GL_RG;
    case 3 : return GL_RGB;
    case 4 : return GL_RGBA;
    default: return 0;
  }
}

uint8_t* Image::get_buffer() const {
  return m_buffer;
}
 
}  // namespace utils