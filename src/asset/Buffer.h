#ifndef _LEARN_OPENGL_SRC_ASSET_BUFFER_H_
#define _LEARN_OPENGL_SRC_ASSET_BUFFER_H_

#include "glad/glad.h"
#include "vector"
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

class UBO : public BufferBase {
  using u_vec = std::vector<GLuint>;

 public:
  UBO(const u_vec& offset_vec, const u_vec& length_vec, GLuint sz, GLuint index);
  ~UBO();
  void set_uniform(GLuint uid, void* data);

  virtual void bind() const override;
  virtual void ubind() const override;  
 private:
  u_vec m_offset_vec;
  u_vec m_length_vec;
};

}  // namespace asset

#endif