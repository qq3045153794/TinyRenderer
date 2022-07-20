#ifndef _LEARN_OPENGL_SRC_ASSET_VAO_H_
#define _LEARN_OPENGL_SRC_ASSET_VAO_H_

#include "asset/Buffer.h"
namespace asset {

class VAO {
 public:
  VAO();
  VAO(const VAO&) = delete;
  VAO& operator=(const VAO&) = delete;
  void set_vbo(const VBO& vbo, GLuint index, GLuint sz, GLuint stride, GLuint offset, GLenum type);
  void set_ibo(const IBO& ibo);

  void bind() const;
  void ubind() const;
  void draw() const;

 private:
  GLuint m_id;
  GLuint m_count;
};

}  // namespace asset

#endif