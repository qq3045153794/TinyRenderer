#include "asset/Buffer.h"

#include "core/Log.h"
namespace asset {

VBO::VBO(GLuint sz, void* data, GLenum usage) : BufferBase() {
  glBindBuffer(GL_ARRAY_BUFFER, m_id);
  glBufferData(GL_ARRAY_BUFFER, sz, data, usage);
}

void VBO::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }
void VBO::ubind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

IBO::IBO(GLuint sz, void* data, GLenum usage, GLuint count) : BufferBase(), m_count(count) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, data, usage);
}

const GLuint& IBO::get_count() const { return m_count; }

void IBO::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IBO::ubind() const { glBindBuffer, (GL_ELEMENT_ARRAY_BUFFER, 0); }

UBO::UBO(const u_vec& offset_vec, const u_vec& length_vec, GLuint sz)
    : BufferBase(), m_offset_vec(offset_vec), m_length_vec(length_vec) {
  glBindBuffer(GL_UNIFORM_BUFFER, m_id);
  glBufferData(GL_UNIFORM_BUFFER, sz, NULL, GL_STATIC_DRAW);
}

void UBO::set_uniform(GLuint uid, void* data) {
  CORE_ASERT(uid < m_offset_vec.size(), "uid more then vector size");
  this->bind();
  glBufferSubData(GL_UNIFORM_BUFFER, m_offset_vec[uid], m_length_vec[uid], data);
  this->ubind();
}

void UBO::set_binding(GLuint uid, const std::string& name, GLuint shader_id) {
  GLuint block_idx = glGetUniformBlockIndex(shader_id, name.c_str());
  if (block_idx == GL_INVALID_INDEX) {
    CORE_ERROR("{} does not identify an active uniform block of program", name);
    return;
  }
  glUniformBlockBinding(shader_id, block_idx, uid);
  glBindBufferBase(GL_UNIFORM_BUFFER, uid, m_id);
}

void UBO::bind() const { glBindBuffer(GL_UNIFORM_BUFFER, m_id); }

void UBO::ubind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

}  // namespace asset
