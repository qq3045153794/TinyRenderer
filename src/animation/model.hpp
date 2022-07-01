#ifndef _LEARN_OPENGL_SRC_ANIMATION_MODEL_H_
#define _LEARN_OPENGL_SRC_ANIMATION_MODEL_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "mesh.hpp"
#include "BoneInfo.hpp"
#include "assimp/Importer.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace gl_animation {

class Model {
 public:
  Model(const std::string& path, bool gamma);
  ~Model();
  void draw(Shader& shader);
  const auto& get_bone_info_map();
  const int& getBoneCount();
 private:
  std::vector<Texture> textures_loaded_;
  std::vector<Mesh> meshes_;
  std::string directory_;
  bool gammaCorrection_;
  std::map<std::string, BoneInfo> bone_info_map_;
  int bone_counter_;

  void load_model(const std::string &path);
  void process_node(aiNode* node, const aiScene* scene);
  void set_vertex_bone_data_to_default(Vertex& vertex);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
  void set_vertex_bone_data(Vertex& vertex, int bone_id, float weight);
  void extract_bone_weight_for_vertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
  GLuint texture_from_file(const char* path, const std::string& directory, bool gamma);
  std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
Model::Model(const std::string& path, bool gamma)
  : gammaCorrection_(gamma) {
  load_model(path);
}


}

#endif