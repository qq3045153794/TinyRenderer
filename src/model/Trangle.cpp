#include "Trangle.h"

Trangle::Trangle(Shader shader, Texture2D texture_2d)
  : position(glm::vec3(0.0, 0.0, 0.0)) {
  this->shader = shader;
  this->texture_2d = texture_2d;
  set_buffers();
}

void Trangle::set_buffers() {
  GLfloat data[] ={
    -1.0, 0.0, -1.0, 0.0, 0.0,
     1.0, 0.0, -1.0, 1.0, 0.0,
     1.0, 0.0,  1.0, 0.0, 0,0,
    -1.0, 0.0,  1.0, 1.0, 0,0,
     0.0, 1,0,  0.0, 0.5, 1.0
  };
  GLuint idx[] = {
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 4, 4,
    0, 1, 2,
    2, 3, 1
  };
  glGenBuffers(1, &this->vao);
  glBindVertexArray(this->vao);
  
  glGenBuffers(1, &this->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

  glGenBuffers(1, &this->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void Trangle::draw() {
  shader.use();
  glm::mat4 model = glm::mat4(1.0);
  model = glm::translate(model, this->position);
  glBindVertexArray(this->vao);
  glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}