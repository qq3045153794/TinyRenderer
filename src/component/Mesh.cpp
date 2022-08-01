#include <iostream>
#include "component/Mesh.h"

static const std::vector<GLuint> va_offset = {
    offsetof(Mesh::Vertex, position), offsetof(Mesh::Vertex, normal), offsetof(Mesh::Vertex, uv)};

static const std::vector<GLuint> va_sz{3U, 3U, 2U};

Mesh::Mesh(primitive obj) {
  switch (obj) {
    case QUAD: create_quat(); break;
    case CUBE: create_cube(); break;
    case SPHERE: create_sphere(); break;
    default: {
      std::cout<<"no obj"<<std::endl;
    }
  }
}

Mesh::~Mesh() {}

void Mesh::create_quat() {
  GLfloat m_data[] = {
    
    0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 0.0, 
    0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0,
    1.0, 0.0, 0.0,  0.0, 0.0, 1.0,  1.0, 0.0, 
    1.0, 1.0, 0.0,  0.0, 0.0, 1.0,  1.0, 1.0,
  };

  std::vector<Vertex> vertex;
  
  GLuint stride = 8U;
  for(size_t i = 0; i < 4; i++){
    GLuint offset = i * stride;
    Vertex vec;
    vec.position = glm::vec3(m_data[offset + 0], m_data[offset + 1], m_data[offset + 2]);
    vec.normal = glm::vec3(m_data[offset + 3], m_data[offset + 4], m_data[offset + 5]);
    vec.uv = glm::vec2(m_data[offset + 6], m_data[offset + 7]);
    vertex.push_back(vec);
  }

  std::vector<GLuint> indices = { 0U, 1U, 2U, 2U, 3U, 1U };
  create_buffer(vertex, indices);
}

void Mesh::draw() const { m_vao->draw(); }

void Mesh::create_cube() {
  // TODO
}
void Mesh::create_sphere() {
  // TODO
}
void Mesh::create_buffer(const std::vector<Vertex>& vertex, const std::vector<GLuint>& indices) {
  
  m_vbo = std::make_shared<asset::VBO>(sizeof(Vertex) * vertex.size(), (void*)vertex.data(), GL_STATIC_DRAW);
  m_ibo = std::make_shared<asset::IBO>(sizeof(GLuint) * indices.size(), (void*)indices.data(), GL_STATIC_DRAW, indices.size());
  m_vao = std::make_shared<asset::VAO>();

  for(size_t i = 0; i < 3; i++) {
    m_vao->set_vbo(*m_vbo, i, va_sz[i], sizeof(Vertex), va_offset[i], GL_FLOAT);   
  }
  m_vao->set_ibo(*m_ibo);

}