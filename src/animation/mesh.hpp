#ifndef _LEARN_OPENGL_SRC_ANIMATION_MESH_HPP_
#define _LEARN_OPENGL_SRC_ANIMATION_MESH_HPP_

#include <vector>
#include <string>
#include "ResourceManager.h"

namespace gl_animation {

constexpr size_t kMaxBone = 4;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  glm::vec3 tangent;
  glm::vec3 bitangent;

  int bone_ids[kMaxBone];
  float weights[kMaxBone];
};

struct Texture {
  GLuint id;
  std::string type;
  std::string path;
};

class Mesh {
 public:
  Mesh() = delete;
  Mesh(const Mesh&) = delete;
  const Mesh& operator=(const Mesh&) = delete;
  ~Mesh();
  Mesh(std::vector<Vertex> vertices_, std::vector<GLuint>indices_, std::vector<Texture> textures_);
  void draw(Shader& shader);
 private:
  void setup_mesh();
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
  std::vector<Texture> textures_;
  GLuint vao_, vbo_, ebo_;

};

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint>indices, std::vector<Texture> textures)
  : vertices_(vertices),
    indices_(indices),
    textures_(textures) {
  setup_mesh();
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &vao_);
  glDeleteBuffers(1, &vbo_);
  glDeleteBuffers(1, &ebo_);
}

void Mesh::setup_mesh() {
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(GLuint), indices_.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
  glEnableVertexAttribArray(3);

  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
  glEnableVertexAttribArray(4);

  glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bone_ids));
  glEnableVertexAttribArray(5);

  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
  glEnableVertexAttribArray(6);
  glBindVertexArray(0);
}

void Mesh::draw(Shader& shader) {
  GLuint diffuse_nr =  1;
  GLuint specular_nr = 1;
  GLuint normal_nr = 1;
  GLuint height_nr = 1;
  shader.use();
  for(size_t i = 0; i < textures_.size(); i++) {
    std::string number;
    std::string name = textures_[i].type;
    if(name == "texture_diffuse") {
      number = std::to_string(diffuse_nr++);
    } else if(name == "texture_specular") {
      number = std::to_string(specular_nr++);
    } else if(name == "texture_normal") {
      number = std::to_string(normal_nr++);
    } else if(name == "texture_height") {
      number = std::to_string(height_nr++);
    }

    glActiveTexture(GL_TEXTURE0 + i);
    shader.set_integer((name + name).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, static_cast<GLuint>(indices_.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

}

#endif