#include <iostream>
#include "component/Mesh.h"
#include "core/Log.h"
static const std::vector<GLuint> va_offset = {
    offsetof(Mesh::Vertex, position), offsetof(Mesh::Vertex, normal), offsetof(Mesh::Vertex, uv)};

static const std::vector<GLuint> va_sz{3U, 3U, 2U};

Mesh::Mesh(primitive obj) {
  switch (obj) {
    case QUAD: create_quat(); break;
    case CUBE: create_cube(); break;
    case SPHERE: create_sphere(); break;
    default: {
      CORE_INFO("MESH primitive INVAILD");
    }
  }
}

Mesh::~Mesh() {}

void Mesh::create_quat() {
  static GLfloat data[] = {
    0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 0.0, 
    0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0,
    1.0, 0.0, 0.0,  0.0, 0.0, 1.0,  1.0, 0.0, 
    1.0, 1.0, 0.0,  0.0, 0.0, 1.0,  1.0, 1.0,
  };

  std::vector<Vertex> vertices;
  
  GLuint stride = 8U;
  for(size_t i = 0; i < 4; i++){
    GLuint offset = i * stride;
    Vertex vec;
    vec.position = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]);
    vec.normal = glm::vec3(data[offset + 3], data[offset + 4], data[offset + 5]);
    vec.uv = glm::vec2(data[offset + 6], data[offset + 7]);
    vertices.push_back(vec);
  }

  std::vector<GLuint> indices = { 0U, 1U, 2U, 2U, 3U, 1U };
  create_buffer(vertices, indices);
}

void Mesh::create_cube() {
  static const GLfloat data[] = {
    // ----position----    -------normal------    ----uv----
    -1.0f, -1.0f, -1.0f,   +0.0f, -1.0f, +0.0f,   0.0f, 0.0f,
    -1.0f, -1.0f, +1.0f,   +0.0f, -1.0f, +0.0f,   0.0f, 1.0f,
    +1.0f, -1.0f, +1.0f,   +0.0f, -1.0f, +0.0f,   1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f,   +0.0f, -1.0f, +0.0f,   1.0f, 0.0f,
    
    -1.0f, +1.0f, -1.0f,   +0.0f, +1.0f, +0.0f,   1.0f, 0.0f,
    -1.0f, +1.0f, +1.0f,   +0.0f, +1.0f, +0.0f,   1.0f, 1.0f,
    +1.0f, +1.0f, +1.0f,   +0.0f, +1.0f, +0.0f,   0.0f, 1.0f,
    +1.0f, +1.0f, -1.0f,   +0.0f, +1.0f, +0.0f,   0.0f, 0.0f,
    
    -1.0f, -1.0f, -1.0f,   +0.0f, +0.0f, -1.0f,   0.0f, 0.0f,
    -1.0f, +1.0f, -1.0f,   +0.0f, +0.0f, -1.0f,   0.0f, 1.0f,
    +1.0f, +1.0f, -1.0f,   +0.0f, +0.0f, -1.0f,   1.0f, 1.0f,
    +1.0f, -1.0f, -1.0f,   +0.0f, +0.0f, -1.0f,   1.0f, 0.0f,
    
    -1.0f, -1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   0.0f, 0.0f,
    -1.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   0.0f, 1.0f,
    +1.0f, +1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   1.0f, 1.0f,
    +1.0f, -1.0f, +1.0f,   +0.0f, +0.0f, +1.0f,   1.0f, 0.0f,
    
    -1.0f, -1.0f, -1.0f,   -1.0f, +0.0f, +0.0f,   0.0f, 0.0f,
    -1.0f, -1.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   0.0f, 1.0f,
    -1.0f, +1.0f, +1.0f,   -1.0f, +0.0f, +0.0f,   1.0f, 1.0f,
    -1.0f, +1.0f, -1.0f,   -1.0f, +0.0f, +0.0f,   1.0f, 0.0f,
    
    +1.0f, -1.0f, -1.0f,   +1.0f, +0.0f, +0.0f,   0.0f, 0.0f,
    +1.0f, -1.0f, +1.0f,   +1.0f, +0.0f, +0.0f,   0.0f, 1.0f,
    +1.0f, +1.0f, +1.0f,   +1.0f, +0.0f, +0.0f,   1.0f, 1.0f,
    +1.0f, +1.0f, -1.0f,   +1.0f, +0.0f, +0.0f,   1.0f, 0.0f
  };
  
  std::vector<Vertex> vertices;

  GLuint stride = 8U;
  for(size_t i = 0; i < 4; i++){
    GLuint offset = i * stride;
    Vertex vec;
    vec.position = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]);
    vec.normal = glm::vec3(data[offset + 3], data[offset + 4], data[offset + 5]);
    vec.uv = glm::vec2(data[offset + 6], data[offset + 7]);
    vertices.push_back(vec);
  }

  // counter-clockwise winding order
  std::vector<GLuint> indices {
      +0, +2, +1,   +0, +3, +2,   +4, +5, +6,
      +4, +6, +7,   +8, +9, 10,   +8, 10, 11,
      12, 15, 14,   12, 14, 13,   16, 17, 18,
      16, 18, 19,   20, 23, 22,   20, 22, 21
  };

  create_buffer(vertices, indices);

}

void Mesh::draw() const { m_vao->draw(); }

void Mesh::create_sphere() {
  // TODO
}

void Mesh::create_buffer(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
  
  m_vbo = std::make_shared<asset::VBO>(sizeof(Vertex) * vertices.size(), (void*)vertices.data(), GL_STATIC_DRAW);
  m_ibo = std::make_shared<asset::IBO>(sizeof(GLuint) * indices.size(), (void*)indices.data(), GL_STATIC_DRAW, indices.size());
  m_vao = std::make_shared<asset::VAO>();

  for(size_t i = 0; i < 3; i++) {
    m_vao->set_vbo(*m_vbo, i, va_sz[i], sizeof(Vertex), 
                   va_offset[i], GL_FLOAT);   
  }
  m_vao->set_ibo(*m_ibo);

}