#include "component/Model.h"

#include <bitset>
#include <chrono>
#include <vector>

#include "component/Mesh.h"
#include "core/Log.h"
namespace component {

Model::Model(const std::string& file_path, Quality quality, bool animated) : m_animated(animated) {
  // clang-format off
  u_int32_t improt_options = static_cast<unsigned int>(quality) 
    | aiProcess_FlipUVs 
    | aiProcess_Triangulate 
    | aiProcess_GenSmoothNormals 
    | aiProcess_FindInvalidData 
    | aiProcess_ValidateDataStructure 
    | aiProcess_CalcTangentSpace 
    | aiProcess_LimitBoneWeights;
  // clang-format on

  Assimp::Importer importer;

  auto start_time = std::chrono::high_resolution_clock::now();

  ai_root = importer.ReadFile(file_path, improt_options);

  if (!ai_root || ai_root->mRootNode == nullptr || ai_root->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
    CORE_ERROR("Failed to import model: {0}", file_path);
    CORE_ERROR("Assimp error: {0}", importer.GetErrorString());
    return;
  }

  process_tree(ai_root->mRootNode, -1);
  process_node(ai_root->mRootNode);

  auto end_time = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> loading_time = end_time - start_time;
  CORE_DEBUG("load model use time {} ms", loading_time.count());
}

void Model::process_tree(aiNode* ai_node, int parent) {}

void Model::process_node(aiNode* ai_node) {
  meshes.reserve(meshes.size() + ai_node->mNumMeshes);

  for (size_t i = 0; i < ai_node->mNumMeshes; i++) {
    size_t mesh_id = ai_node->mMeshes[i];
    aiMesh* ai_mesh = ai_root->mMeshes[mesh_id];
    process_mesh(ai_mesh);
  }

  for (size_t i = 0; i < ai_node->mNumChildren; i++) {
    aiNode* child_node = ai_node->mChildren[i];
    process_node(child_node);
  }
}

void Model::process_mesh(aiMesh* ai_mesh) {
  std::vector<Mesh::Vertex> vertices;
  std::vector<GLuint> indices;
  std::bitset<6> local_format;

  vertices.reserve(ai_mesh->mNumVertices);
  indices.reserve(ai_mesh->mNumFaces * 3);

  local_format.set(0, ai_mesh->HasPositions());
  local_format.set(1, ai_mesh->HasNormals());
  local_format.set(2, ai_mesh->HasTextureCoords(0));
  local_format.set(3, ai_mesh->HasTextureCoords(1) && ai_mesh->GetNumUVChannels() > 1);
  local_format.set(4, ai_mesh->HasTangentsAndBitangents());
  local_format.set(5, ai_mesh->HasTangentsAndBitangents());

  static bool warned = false;
  if (vtx_format != local_format && !warned) {
    CORE_WARN("Inconsistent vertex format! Some meshes have attributes missing...");
    warned = true;
  }

  vtx_format |= local_format;

  for (size_t i = 0; i < ai_mesh->mNumVertices; i++) {
    Mesh::Vertex vertex;
    vertex.bone_id = glm::ivec4(-1);

    if (local_format.test(0)) {
      aiVector3D& ai_pos = ai_mesh->mVertices[i];
      vertex.position = glm::vec3(ai_pos.x, ai_pos.y, ai_pos.z);
    }

    if (local_format.test(1)) {
      aiVector3D& ai_normat = ai_mesh->mNormals[i];
      vertex.normal = glm::vec3(ai_normat.x, ai_normat.y, ai_normat.z);
    }

    if (local_format.test(2)) {
      aiVector3D& ai_uv = ai_mesh->mTextureCoords[0][i];  // 1st UV set
      vertex.uv = glm::vec2(ai_uv.x, ai_uv.y);
    }

    if (local_format.test(3)) {
      aiVector3D& ai_uv2 = ai_mesh->mTextureCoords[1][i];  // 2nd UV set
      vertex.uv2 = glm::vec2(ai_uv2.x, ai_uv2.y);
    }

    // tangents and bitangents 成对出现
    if (local_format.test(4) && local_format.test(5)) {
      aiVector3D& ai_tangent = ai_mesh->mTangents[i];
      aiVector3D& ai_binormal = ai_mesh->mBitangents[i];
      vertex.tangent = glm::vec3(ai_tangent.x, ai_tangent.y, ai_tangent.z);
      vertex.binormal = glm::vec3(ai_binormal.x, ai_binormal.y, ai_binormal.z);
    }

    vertices.push_back(vertex);
  }

  for (size_t i = 0; i < ai_mesh->mNumFaces; i++) {
    aiFace& triangle = ai_mesh->mFaces[i];
    CORE_ASERT(triangle.mNumIndices == 3, "This polygon is not a triangle!");

    // assimp默认连接顺序时CCW (逆时针)
    indices.push_back(triangle.mIndices[0]);
    indices.push_back(triangle.mIndices[1]);
    indices.push_back(triangle.mIndices[2]);
  }

  if(m_animated) {
    // TODO
  }
}

void process_material(aiMaterial* ai_material, const Mesh& mesh) {
  
}

}  // namespace component