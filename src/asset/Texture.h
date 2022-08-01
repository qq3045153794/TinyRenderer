#ifndef _LEARN_OPENGL_SRC_ASSET_TEXTURE_H_
#define _LEARN_OPENGL_SRC_ASSET_TEXTURE_H_

#include <vector>
#include "glad/glad.h"


namespace asset {

class Texture {
 public:
  Texture(const GLchar* path);
  Texture(const std::vector<GLchar*>& path_vec);
  Texture(const GLchar* path, GLuint sampler) {};
  ~Texture();
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