#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "utils/image.h"
#include "glm/glm.hpp"
#include "stb_image.h"
#include "glad/glad.h"
#include "core/Log.h"
namespace utils {
Image::Image(const std::string& img_path, bool flip) : m_width(0), m_height(0), n_channels(0) {

  CORE_INFO("Loading image from {}...", img_path);

  stbi_set_flip_vertically_on_load(flip);

  m_is_hdr = stbi_is_hdr(img_path.c_str());
  if (m_is_hdr) {
    float* buffer = stbi_loadf(img_path.c_str(), &m_width, &m_height, &n_channels, 0);

    int n_pixels = m_width * m_height;

    float luminance_min = std::numeric_limits<float>::max();

    float luminance_max = std::numeric_limits<float>::min();
    for (int i = 0; i < n_pixels; i ++) {
      const float* pixels_ptr = buffer + i * 3;
      glm::vec3 color(pixels_ptr[0], pixels_ptr[1], pixels_ptr[2]);
      auto luminance = glm::dot(color, glm::vec3(0.2126,0.7152,0.0722));
      luminance_max = std::max(luminance_max, luminance);
      luminance_min = std::min(luminance_min, luminance);
    }

    auto luminance_diff = luminance_max - luminance_min;
    CORE_TRACE("HDR Image luminance report...");
    CORE_DEBUG("luminance max {}, luminance min {}", luminance_max, luminance_min);

    if (luminance_diff > 1000.0) {
      CORE_WARN("Input HDR iamge too bright...");
    }

    m_buffer = (uint8_t*)(buffer);
  } else {
    m_buffer = stbi_load(img_path.c_str(), &m_width, &m_height, &n_channels, 0);
  }
  stbi_set_flip_vertically_on_load(false);
  if(!m_buffer){
    CORE_ERROR("Texture failed to load at path: {}", img_path);
  }
}


bool Image::is_hdr() const {
  return m_is_hdr;
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
  // clang-format off
  switch(n_channels) {
    case 1 : return GL_RED;
    case 2 : return GL_RG;
    case 3 : return GL_RGB;
    case 4 : return GL_RGBA;
    default: return 0;
  }
  // clang-format on
}

GLuint Image::get_ine_format() const {
  // clang-format off
  switch(n_channels) {
    case 1 : return GL_RED;
    case 2 : return GL_RG;
    case 3 : return GL_RGB;
    case 4 : return GL_RGBA;
    default: return 0;
  }
  // clang-format on
}

uint8_t* Image::get_buffer() const {
  return m_buffer;
}
 
}  // namespace utils
