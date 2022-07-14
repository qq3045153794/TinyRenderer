#ifndef _LEARN_OPENGL_SRC_ANIMATION_MODEL_H_
#define _LEARN_OPENGL_SRC_ANIMATION_MODEL_H_

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "AssimToGlm.hpp"
#include "BoneInfo.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "mesh.hpp"
#include "stb_image.h"

namespace gl_animation {

class Model {
 public:
  Model(const std::string& path, bool gamma);
  ~Model();
  void draw(Shader& shader);
  auto& get_bone_info_map();
  const int& getBoneCount();

 private:
  std::vector<Texture> textures_loaded_;
  std::vector<Mesh> meshes_;
  std::string directory_;
  bool gammaCorrection_;
  std::map<std::string, BoneInfo> bone_info_map_;
  int bone_counter_ = 0;

  void load_model(const std::string& path);
  void process_node(aiNode* node, const aiScene* scene);
  void set_vertex_bone_data_to_default(Vertex& vertex);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
  void set_vertex_bone_data(Vertex& vertex, int bone_id, float weight);
  void extract_bone_weight_for_vertices(std::vector<Vertex>& vertices, aiMesh* mesh,
                                        const aiScene* scene);
  GLuint texture_from_file(const char* path, const std::string& directory, bool gamma);
  std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type,
                                              std::string typeName);
};

Model::Model(const std::string& path, bool gamma) : gammaCorrection_(gamma) { load_model(path); }

void Model::load_model(const std::string& path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  // if is Not Zero
  {
    std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
    std::cout << scene << std::endl;
    std::cout << (scene->mFlags) << std::endl;
    std::cout << AI_SCENE_FLAGS_INCOMPLETE << std::endl;
    std::cout << scene->mRootNode << std::endl;
    std::cout << "model" << std::endl;

    std::cout << "end model" << std::endl;
    return;
  }
  directory_ = path.substr(0, path.find_last_of('/'));
  process_node(scene->mRootNode, scene);
}

// 递归将mesh 放入vector
void Model::process_node(aiNode* node, const aiScene* scene) {
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(process_mesh(mesh, scene));
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  // add positon normal tex
  for (size_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    set_vertex_bone_data_to_default(vertex);
    vertex.position = AssimpTOGlm::get_glm_vec(mesh->mVertices[i]);
    vertex.normal = AssimpTOGlm::get_glm_vec(mesh->mNormals[i]);

    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.tex_coords = vec;
    } else {
      vertex.tex_coords = glm::vec3(0.0f, 0.0f, 0.0f);
    }
    vertices.push_back(vertex);
  }

  // add indices
  for (size_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (size_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // add texture
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
  std::vector<Texture> diffuse_maps =
      load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

  std::vector<Texture> specular_maps =
      load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

  std::vector<Texture> normal_maps =
      load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
  textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());

  std::vector<Texture> height_maps =
      load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
  textures.insert(textures.end(), height_maps.begin(), height_maps.end());

  // add bone
  extract_bone_weight_for_vertices(vertices, mesh, scene);

  return Mesh(vertices, indices, textures);
}

void Model::set_vertex_bone_data_to_default(Vertex& vertex) {
  for (int i = 0; i < kMaxBone; i++) {
    vertex.bone_ids[i] = -1;
    vertex.weights[i] = 0.0f;
  }
}

void Model::set_vertex_bone_data(Vertex& vertex, int bone_id, float weight) {
  for (size_t i = 0; i < kMaxBone; ++i) {
    if (vertex.bone_ids[i] < 0) {
      vertex.weights[i] = weight;
      vertex.bone_ids[i] = bone_id;
      break;
    }
  }
}

GLuint Model::texture_from_file(const char* path, const std::string& directory, bool gamma) {
  std::string filename = std::string(path);
  filename = directory + '/' + filename;
  GLuint textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

std::vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type,
                                                   std::string typeName) {
  std::vector<Texture> textures;
  for (size_t i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for (size_t j = 0; j < textures_loaded_.size(); j++) {
      if (std::strcmp(textures_loaded_[j].path.data(), str.C_Str()) == 0) {
        textures.push_back(textures_loaded_[j]);
        skip = true;
        break;
      }
    }

    if (!skip) {
      Texture texture;
      texture.id = texture_from_file(str.C_Str(), directory_, false);
      texture.type = typeName;
      texture.path = str.C_Str();

      textures.push_back(texture);
      textures_loaded_.push_back(texture);
    }
  }
  return textures;
}

void Model::extract_bone_weight_for_vertices(std::vector<Vertex>& vertices, aiMesh* mesh,
                                             const aiScene* scene) {
  auto& bone_info_map = bone_info_map_;
  int& bone_count = bone_counter_;

  for (size_t bone_index = 0; bone_index < mesh->mNumBones; ++bone_index) {
    int bone_id = -1;
    std::string bone_name = mesh->mBones[bone_index]->mName.C_Str();
    if (bone_info_map.find(bone_name) == bone_info_map.end()) {
      BoneInfo new_bone_info;
      new_bone_info.id = bone_count;
      new_bone_info.offset =
          AssimpTOGlm::convert_matrix_to_glm_format(mesh->mBones[bone_index]->mOffsetMatrix);
      bone_info_map[bone_name] = new_bone_info;
      bone_id = bone_count;
      bone_count++;
    } else {
      bone_id = bone_info_map[bone_name].id;
    }
    auto weights = mesh->mBones[bone_index]->mWeights;
    int num_weights = mesh->mBones[bone_index]->mNumWeights;

    for (size_t weight_index = 0; weight_index < num_weights; ++weight_index) {
      int vertex_id = weights[weight_index].mVertexId;
      float weight = weights[weight_index].mWeight;
      set_vertex_bone_data(vertices[vertex_id], bone_id, weight);
    }
  }
}

void Model::draw(Shader& shader) {
  for (size_t i = 0; i < meshes_.size(); i++) {
    meshes_[i].draw(shader);
  }
}

auto& Model::get_bone_info_map() { return bone_info_map_; }

const int& Model::getBoneCount() { return bone_counter_; }

}  // namespace gl_animation

#endif