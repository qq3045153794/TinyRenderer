#ifndef _LEARN_OPENGL_SRC_ASSET_BUFFER_H_
#define _LEARN_OPENGL_SRC_ASSET_BUFFER_H_

#include "glad/glad.h"
namespace asset {

class BufferBase {
 public:
  BufferBase() { glGenBuffers(1, &m_id); };
  virtual ~BufferBase() { glDeleteBuffers(1, &m_id); };
  BufferBase(const BufferBase&) = delete;
  BufferBase& operator=(const BufferBase&) = delete;

  virtual void bind() const = 0;
  virtual void ubind() const = 0;

 protected:
  GLuint m_id;
};

class VBO : public BufferBase {
 public:
  VBO(GLuint sz, void* data, GLenum usage);
  virtual ~VBO(){};
  virtual void bind() const override;
  virtual void ubind() const override;
};

class IBO : public BufferBase {
 public:
  IBO(GLuint sz, void* data, GLenum usage, GLuint count);
  virtual ~IBO(){};

  const GLuint& get_count() const;

  virtual void bind() const override;
  virtual void ubind() const override;

 private:
  GLuint m_count;
};

}  // namespace asset

#endif