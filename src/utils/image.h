#ifndef _LEARN_OPENGL_SRC_UTILS_IMAGE_H_
#define _LEARN_OPENGL_SRC_UTILS_IMAGE_H_

#include <string>

#include "glad/glad.h"
namespace utils {

class Image {
 public:
  Image(const std::string& img_path, bool flip = false);
  ~Image();
  Image(const Image&) = delete;
  Image& operator=(const Image&) = delete;
  GLuint get_width() const;
  GLuint get_height() const;
  GLuint get_img_format() const;
  GLuint get_ine_format() const;
  uint8_t* get_buffer() const;

 private:
  GLint m_width, m_height, n_channels;
  uint8_t* m_buffer;
};

}  // namespace utils

#endif
