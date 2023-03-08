#include "asset/VAO.h"

#include <iostream>
namespace asset {

GLuint static g_cur_bound_vao = 0;

VAO::VAO() : m_count(0) { glGenVertexArrays(1, &m_id); }

VAO::~VAO() { glDeleteBuffers(1, &m_id); }

void VAO::set_vbo(const VBO& vbo, GLuint index, GLuint sz, GLuint stride,
                  GLuint offset, GLenum type) {
  this->bind();
  vbo.bind();
  glVertexAttribPointer(index, sz, type, GL_FALSE, stride,
                        reinterpret_cast<void*>(offset));
  glEnableVertexAttribArray(index);
  this->ubind();
  vbo.ubind();
}

void VAO::set_ibo(const IBO& ibo) {
  this->bind();
  ibo.bind();
  this->ubind();
  ibo.ubind();
  m_count = ibo.get_count();
}

void VAO::bind() const {
  if (g_cur_bound_vao != m_id) {
    g_cur_bound_vao = m_id;
    glBindVertexArray(m_id);
  }
}

void VAO::ubind() const {
  if (g_cur_bound_vao == m_id) {
    g_cur_bound_vao = 0;
    glBindVertexArray(0);
  }
}

void VAO::draw() const {
  this->bind();
  glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
  this->ubind();
}

void VAO::draw(GLuint offset, GLuint count) const {
  this->bind();
  glDrawArrays(GL_TRIANGLES, offset, count);
  this->ubind();
}

}  // namespace asset
