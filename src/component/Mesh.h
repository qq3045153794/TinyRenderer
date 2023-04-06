#ifndef _LEARN_OPENGL_SRC_COMPONENT_MESH_H_
#define _LEARN_OPENGL_SRC_COMPONENT_MESH_H_

#include <memory>

#include "asset/Buffer.h"
#include "asset/VAO.h"
#include "glm/glm.hpp"
namespace component {

class Mesh {
 public:
  enum primitive { QUAD = 0, CUBE = 1, SPHERE = 2, NONE = 3 };

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    glm::vec2 uv2;
    glm::vec3 tangent;
    glm::vec3 binormal;
    glm::ivec4 bone_id;
    glm::vec4 bone_wt;
  };

  Mesh(primitive obj);
  Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
  ~Mesh();

  void draw() const;

  void set_material_id(GLuint mid) const;
  std::string get_primitive_strings();
  static primitive cast_primitive(const std::string& primitive);

  primitive m_obj{NONE};
  mutable GLuint material_id;

 private:
  std::shared_ptr<asset::VAO> m_vao;
  std::shared_ptr<asset::VBO> m_vbo;
  std::shared_ptr<asset::IBO> m_ibo;

  void create_quat();
  void create_cube();
  void create_sphere();
  void create_buffer(const std::vector<Vertex>& vertex,
                     const std::vector<GLuint>& indices);
};

}  // namespace component
#endif
