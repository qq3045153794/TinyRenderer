#include "asset/Buffer.h"

#include "core/Log.h"

namespace asset {

static GLuint g_curr_bound_vbo = 0;
static GLuint g_curr_bound_ibo = 0;
static GLuint g_curr_bound_ubo = 0;

VBO::VBO(GLuint sz, void* data, GLenum usage) : BufferBase() {
  glBindBuffer(GL_ARRAY_BUFFER, m_id);
  glBufferData(GL_ARRAY_BUFFER, sz, data, usage);
}

void VBO::bind() const {
  if (g_curr_bound_vbo != m_id) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    g_curr_bound_vbo = m_id;
  }
}
void VBO::ubind() const {
  if (g_curr_bound_vbo == m_id) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    g_curr_bound_vbo = 0;
  }
}

IBO::IBO(GLuint sz, void* data, GLenum usage, GLuint count) : BufferBase(), m_count(count) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sz, data, usage);
}

const GLuint& IBO::get_count() const { return m_count; }

void IBO::bind() const {
  if (g_curr_bound_ibo != m_id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    g_curr_bound_ibo = m_id;
  }
}

void IBO::ubind() const {
  if (g_curr_bound_ibo == m_id) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    g_curr_bound_ibo = 0;
  }
}

UBO::UBO(const u_vec& offset_vec, const u_vec& length_vec, GLuint sz)
    : BufferBase(), m_offset_vec(offset_vec), m_length_vec(length_vec) {
  glBindBuffer(GL_UNIFORM_BUFFER, m_id);
  glBufferData(GL_UNIFORM_BUFFER, sz, nullptr, GL_STATIC_DRAW);
}

void UBO::set_uniform(GLuint uid, void* data) {
  CORE_ASERT(uid < m_offset_vec.size(), "uid more then vector size");
  this->bind();
  glBufferSubData(GL_UNIFORM_BUFFER, m_offset_vec[uid], m_length_vec[uid], data);
  // this->ubind();
}

void UBO::set_multi_uniform(GLuint uid, GLuint id, void* data) {
  CORE_ASERT(uid < m_offset_vec.size(), "uid more then vector size");
  this->bind();
  // 16为步长 参考shader
  glBufferSubData(GL_UNIFORM_BUFFER, m_offset_vec[uid] + id * 16, m_length_vec[uid], data);
}

void UBO::set_uid(GLuint uid) { m_uid = uid; }
GLuint UBO::Uid() { return m_uid; }

void UBO::set_binding(GLuint uid, const std::string& name, GLuint shader_id) {
  GLuint block_idx = glGetUniformBlockIndex(shader_id, name.c_str());
  if (block_idx == GL_INVALID_INDEX) {
    // 暂时注释
    // CORE_ERROR("{} does not identify an active uniform block of program", name);
    return;
  }
  glUniformBlockBinding(shader_id, block_idx, uid);
  glBindBufferBase(GL_UNIFORM_BUFFER, uid, m_id);
}

void UBO::bind() const {
  if (g_curr_bound_ubo != m_id) {
    glBindBuffer(GL_UNIFORM_BUFFER, m_id);
    g_curr_bound_ubo = m_id;
  }
}

void UBO::ubind() const {
  if (g_curr_bound_ubo == m_id) {
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    g_curr_bound_ubo = 0;
  }
}

}  // namespace asset
