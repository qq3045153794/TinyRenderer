#ifndef _SRC_ASSET_VAO_H_
#define _SRC_ASSET_VAO_H_

#include "asset/Buffer.h"
namespace asset {

class VAO {
 public:
  VAO();
  VAO(const VAO&) = delete;
  VAO& operator=(const VAO&) = delete;
  ~VAO();
  void set_vbo(const VBO& vbo, GLuint index, GLuint sz, GLuint stride,
               GLuint offset, GLenum type);
  void set_ibo(const IBO& ibo);
  GLint get_id() { return m_id; }

  void bind() const;
  void ubind() const;
  void draw() const;
  void draw(GLuint offset, GLuint count) const;

 private:
  GLuint m_id;
  GLuint m_count;
};

}  // namespace asset

#endif
