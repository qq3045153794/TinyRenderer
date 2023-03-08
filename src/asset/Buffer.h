#ifndef _SRC_ASSET_BUFFER_H_
#define _SRC_ASSET_BUFFER_H_

#include <string>
#include <vector>

#include "glad/glad.h"
namespace asset {

class BufferBase {
 public:
  BufferBase() { glGenBuffers(1, &m_id); }
  virtual ~BufferBase() { glDeleteBuffers(1, &m_id); }
  BufferBase(const BufferBase&) = delete;
  BufferBase& operator=(const BufferBase&) = delete;

  GLuint get_id() const { return m_id; }
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
  UBO() = default;
  UBO(const u_vec& offset_vec, const u_vec& length_vec, GLuint sz);
  virtual ~UBO(){};
  void set_binding(GLuint uid, const std::string& name, GLuint shader_id);
  void set_uniform(GLuint uid, void* data);

  virtual void bind() const override;
  virtual void ubind() const override;

 private:
  u_vec m_offset_vec;
  u_vec m_length_vec;
};

}  // namespace asset

#endif
