#ifndef _LEARN_OPENGL_SRC_MODEL_IMAGE_H_
#define _LEARN_OPENGL_SRC_MODEL_IMAGE_H_

#include <memory>
#include <vector>

#include "ResourceManager.h"
class Image {
 public:
  Image(glm::vec2 sz, glm::vec3 postion, glm::quat rotation);
  ~Image();
  Image(const Image&) = delete;
  const Image& operator=(const Image&) = delete;
  void draw(Shader& shader);
  static std::shared_ptr<Image> create_image_model(glm::vec2 sz, glm::vec3 position,
                                                   glm::quat rotation);

 private:
  void setup_buffer();
  glm::vec2 m_sz;
  glm::vec3 m_position;
  glm::quat m_rotation;
  GLuint m_vao;
  GLuint m_vbo;
};

#endif