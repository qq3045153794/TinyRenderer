/*
 * @Author       : xietao
 * @Date         : 2022-09-14 14:38:12
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-27 17:08:24
 * @FilePath     : /Learn_OpenGL/src/asset/Texture.h
 * @brief  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _SRC_ASSET_TEXTURE_H_
#define _SRC_ASSET_TEXTURE_H_

#include <string>
#include <vector>

#include "glad/glad.h"

namespace asset {

class Texture {
 public:
  Texture(const std::vector<std::string>& path_vec);
  Texture(const GLchar* path, bool flip = false);
  Texture(const GLchar* path, GLuint sampler) {}
  Texture(GLuint width, GLuint height);
  ~Texture();
  GLuint get_id();
  void bind(GLuint unit) const;
  void ubind(GLuint unit) const;

 private:
  void set_sampler_state();

  GLuint m_id;
  GLuint m_width, m_height;
  GLuint m_internal_format;
  GLuint m_image_format;
  GLuint m_target;
};

}  // namespace asset

#endif