#include "asset/Buffer.h"

namespace asset {

VBO::VBO(GLuint sz, void* data, GLenum usage) : BufferBase() {
  glBindBuffer(GL_ARRAY_BUFFER, m_id);
  glBufferData(GL_ARRAY_BUFFER, sz, data, usage);
}

void VBO::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_id); }
void VBO::ubind() const { glBindBuffer(0, m_id); }

IBO::IBO(GLuint sz, void* data, GLenum usage, GLuint count) : BufferBase() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, data, usage);
}

const GLuint& IBO::get_count() const { return m_count; }

void IBO::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }

void IBO::ubind() const { glBindBuffer(0, m_id); }

}  // namespace asset