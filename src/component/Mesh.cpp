#include "component/Mesh.h"

#include <cmath>
#include <iostream>

#include "core/Log.h"
namespace component {

static const std::vector<GLuint> va_offset = {
    offsetof(Mesh::Vertex, position), offsetof(Mesh::Vertex, normal),
    offsetof(Mesh::Vertex, uv),       offsetof(Mesh::Vertex, uv2),
    offsetof(Mesh::Vertex, tangent),  offsetof(Mesh::Vertex, binormal),
    offsetof(Mesh::Vertex, bone_id),  offsetof(Mesh::Vertex, bone_wt)};

static const std::vector<GLuint> va_sz{3U, 3U, 2U, 2U, 3U, 3U, 4U, 4U};

Mesh::Mesh(primitive obj) {
  // clang-format off
  switch (obj) {
    case QUAD: create_quat(); break;
    case CUBE: create_cube(); break;
    case SPHERE: create_sphere(); break;
    default: {
      CORE_WARN("MESH primitive INVAILD");
    }
  }
  // clang-format on
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
  create_buffer(vertices, indices);
  material_id = m_vao->get_id();
}

Mesh::~Mesh() {}

void Mesh::create_quat() {
  // clang-format off
  static GLfloat data[] = {
    0.0, 0.0, 0.0,  0.0, 0.0, 1.0,  0.0, 0.0,   // left bottom
    0.0, 1.0, 0.0,  0.0, 0.0, 1.0,  0.0, 1.0,   // left top
    1.0, 0.0, 0.0,  0.0, 0.0, 1.0,  1.0, 0.0,   // right bottom
    1.0, 1.0, 0.0,  0.0, 0.0, 1.0,  1.0, 1.0,   // right top
  };
  // clang-format on

  std::vector<Vertex> vertices;

  GLuint stride = 8U;
  for (size_t i = 0; i < 4; i++) {
    GLuint offset = i * stride;
    Vertex vec;
    vec.position = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]);
    vec.normal = glm::vec3(data[offset + 3], data[offset + 4], data[offset + 5]);
    vec.uv = glm::vec2(data[offset + 6], data[offset + 7]);
    vertices.push_back(vec);
  }
  // clang-format off
  std::vector<GLuint> indices = { 
    0U, 2U, 1U, // CCW 
    2U, 3U, 1U  // CCW
  };
  // clang-format on
  create_buffer(vertices, indices);
}

void Mesh::create_cube() {
  // clang-format off
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
  // clang-format on

  std::vector<Vertex> vertices;

  GLuint stride = 8U;
  GLuint n_vec = 26;
  for (size_t i = 0; i < n_vec; i++) {
    GLuint offset = i * stride;
    Vertex vec;
    vec.position = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]);
    vec.normal = glm::vec3(data[offset + 3], data[offset + 4], data[offset + 5]);
    vec.uv = glm::vec2(data[offset + 6], data[offset + 7]);
    vertices.push_back(vec);
  }

  // clang-format off
  std::vector<GLuint> indices {
      +0, +2, +1,   +0, +3, +2,   +4, +5, +6,
      +4, +6, +7,   +8, +9, 10,   +8, 10, 11,
      12, 15, 14,   12, 14, 13,   16, 17, 18,
      16, 18, 19,   20, 23, 22,   20, 22, 21
  };
  // clang-format on

  create_buffer(vertices, indices);
  CORE_TRACE("create cube");
}

void Mesh::create_sphere() {
  const float radius = 1.0;
  const float PI = std::acos(-1);
  const float PI_2 = std::acos(-1) * 0.5;

  // default LOD = 100x100 mesh grid size
  unsigned int n_rows = 100;
  unsigned int n_cols = 100;
  unsigned int n_verts = (n_rows + 1) * (n_cols + 1);
  unsigned int n_tris = n_rows * n_cols * 2;

  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  vertices.reserve(n_verts);
  indices.reserve(n_tris * 3);

  for (unsigned int col = 0; col <= n_cols; ++col) {
    for (unsigned int row = 0; row <= n_rows; ++row) {
      // unscaled uv coordinates ~ [0, 1]
      float u = static_cast<float>(col) / n_cols;
      float v = static_cast<float>(row) / n_rows;

      float theta = PI * v - PI_2;  // ~ [-PI/2, PI/2], latitude from south to north pole
      float phi = PI * 2 * u;       // ~ [0, 2PI], longitude around the equator circle
      // 球的极坐标方程
      float x = cos(phi) * cos(theta);
      float y = sin(theta);
      float z = sin(phi) * cos(theta) * (-1);

      // for a unit sphere centered at the origin, normal = position
      // binormal is normal rotated by 90 degrees along the latitude (+theta)
      // theta += PI_2;
      // float r = cos(phi) * cos(theta);
      // float s = sin(theta);
      // float t = sin(phi) * cos(theta) * (-1);

      Vertex vertex{};
      vertex.position = glm::vec3(x, y, z) * radius;
      vertex.normal = glm::vec3(x, y, z);
      vertex.uv = glm::vec2(u, v);
      // vertex.binormal = glm::vec3(r, s, t);
      // vertex.tangent  = glm::cross(vertex.binormal, vertex.normal);

      vertices.push_back(vertex);
    }
  }

  for (unsigned int col = 0; col < n_cols; ++col) {
    for (unsigned int row = 0; row < n_rows; ++row) {
      auto index = col * (n_rows + 1);

      // counter-clockwise winding order
      indices.push_back(index + row + 1);
      indices.push_back(index + row);
      indices.push_back(index + row + 1 + n_rows);

      // counter-clockwise winding order
      indices.push_back(index + row + 1 + n_rows + 1);
      indices.push_back(index + row + 1);
      indices.push_back(index + row + 1 + n_rows);
    }
  }

  create_buffer(vertices, indices);

  CORE_TRACE("create sphere");
}

void Mesh::draw() const { m_vao->draw(); }

void Mesh::create_buffer(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices) {
  m_vbo = std::make_shared<asset::VBO>(sizeof(Vertex) * vertices.size(), (void*)vertices.data(),
                                       GL_STATIC_DRAW);
  m_ibo = std::make_shared<asset::IBO>(sizeof(GLuint) * indices.size(), (void*)indices.data(),
                                       GL_STATIC_DRAW, indices.size());
  m_vao = std::make_shared<asset::VAO>();

  for (size_t i = 0; i < 8; i++) {
    GLenum va_type = i == 6 ? GL_INT : GL_FLOAT;
    m_vao->set_vbo(*m_vbo, i, va_sz[i], sizeof(Vertex), va_offset[i], va_type);
  }
  m_vao->set_ibo(*m_ibo);
}

void Mesh::set_material_id(GLuint mid) const { material_id = mid; }

}  // namespace component