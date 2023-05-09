#include "component/Model.h"

#include <component/Animator.h>

#include <algorithm>
#include <bitset>
#include <chrono>
#include <vector>

#include "component/Material.h"
#include "component/Mesh.h"
#include "core/Log.h"
namespace component {

static constexpr unsigned int max_vtx_bones = 4;

Node::Node(int nid, int pid, const std::string& name) : nid(nid), pid(pid), bid(-1), alive(0), name(name) {
  CORE_ASERT(nid >= 0 && pid < nid, "Parent node is not procssed before its chidren!");
}

bool Node::is_bone() const { return bid >= 0; }

bool Node::animated() const { return (bid >= 0) && alive; }

static inline glm::mat4 AssimpMat2GLM(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }

Model::Model(const std::string& file_path, Quality quality, bool animated) : model_filepath(file_path), m_animated(animated) {
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
  importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_ANIMATIONS, false);

  auto start_time = std::chrono::high_resolution_clock::now();

  ai_root = importer.ReadFile(file_path, improt_options);

  if (!ai_root || ai_root->mRootNode == nullptr || ai_root->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
    CORE_ERROR("Failed to import model: {0}", file_path);
    CORE_ERROR("Assimp error: {0}", importer.GetErrorString());
    std::runtime_error("Failed to import model");
    return;
  }

  process_tree(ai_root->mRootNode, -1);
  process_node(ai_root->mRootNode);

  auto end_time = std::chrono::high_resolution_clock::now();

  CORE_TRACE("Generating model loading report...... (for reference)");
  CORE_TRACE("-----------------------------------------------------");

  std::chrono::duration<double, std::milli> loading_time = end_time - start_time;
  CORE_DEBUG("load model use time {} ms", loading_time.count());

  CORE_DEBUG("total # of meshes:     {0}", n_meshes);
  CORE_DEBUG("total # of vertices:   {0:.2f}k ({1})", n_verts * 0.001f, n_verts);
  CORE_DEBUG("total # of triangles:  {0:.2f}k ({1})", n_tris * 0.001f, n_tris);
  CORE_TRACE("-----------------------------------------------------");

  CORE_DEBUG("vertex has position ? [{0}]", vtx_format.test(0) ? "Y" : "N");
  CORE_DEBUG("vertex has normal   ? [{0}]", vtx_format.test(1) ? "Y" : "N");
  CORE_DEBUG("vertex has uv set 1 ? [{0}]", vtx_format.test(2) ? "Y" : "N");
  CORE_DEBUG("vertex has uv set 2 ? [{0}]", vtx_format.test(3) ? "Y" : "N");
  CORE_DEBUG("vertex has tan/btan ? [{0}]", vtx_format.test(4) ? "Y" : "N");
  CORE_TRACE("-----------------------------------------------------");

  CORE_DEBUG("internal materials");
  for (const auto& it : materials_cache) {
    CORE_DEBUG("{} (id = {})", it.first, it.second);
  }
  CORE_TRACE("-----------------------------------------------------");
  CORE_DEBUG("internal meshes");
  for (const auto& it : meshes_name) {
    CORE_DEBUG("mesh name : {}", it);
  }
  CORE_TRACE("-----------------------------------------------------");
  CORE_DEBUG("internal Node");
  for (const auto& node : nodes) {
    CORE_DEBUG("Node name : {} id : {} , parent : {}", node.name, node.nid, node.pid);
  }
  CORE_TRACE("-----------------------------------------------------");
}

Model::Model(Model&& rhs) = default;
Model& Model::operator=(Model&& rhs) = default;
Model::~Model() = default;

void Model::process_tree(aiNode* ai_node, int parent) {
  aiString& ai_name = ai_node->mName;
  auto& node = nodes.emplace_back(n_nodes++, parent, ai_name.length == 0 ? "unnamed" : ai_name.C_Str());
  node.n2p = AssimpMat2GLM(ai_node->mTransformation);
  int next_parent = n_nodes - 1;
  for (std::size_t i = 0; i < ai_node->mNumChildren; i++) {
    aiNode* child_node = ai_node->mChildren[i];
    process_tree(child_node, next_parent);
  }
}

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

    // 统计顶点数
    n_verts++;
  }

  for (size_t i = 0; i < ai_mesh->mNumFaces; i++) {
    aiFace& triangle = ai_mesh->mFaces[i];
    CORE_ASERT(triangle.mNumIndices == 3, "This polygon is not a triangle!");

    // assimp默认连接顺序时CCW (逆时针)
    indices.push_back(triangle.mIndices[0]);
    indices.push_back(triangle.mIndices[1]);
    indices.push_back(triangle.mIndices[2]);
    // 统计三角形面数
    n_tris++;
  }

  if (m_animated) {
    for (std::size_t i = 0; i < ai_mesh->mNumBones; i++) {
      aiBone* ai_bone = ai_mesh->mBones[i];
      std::string name = ai_bone->mName.C_Str();
      auto it = std::find_if(nodes.begin(), nodes.end(), [&name](const Node& node) { return name == node.name; });
      CORE_ASERT(it != nodes.end(), "Invaild bone, cannot find a match in the nodes hierarchy!");
      Node& node = nodes[it->nid];
      if (node.bid < 0) {
        node.m2n = AssimpMat2GLM(ai_bone->mOffsetMatrix);
        node.bid = n_bones++;
      }

      for (int j = 0; j < ai_bone->mNumWeights; j++) {
        unsigned int vtx_id = ai_bone->mWeights[j].mVertexId;
        const float weight = ai_bone->mWeights[j].mWeight;
        CORE_ASERT(vtx_id < vertices.size(), "Vertex id out of bound!");
        auto& vertex = vertices[vtx_id];
        bool full = glm::all(glm::greaterThanEqual(vertex.bone_id, glm::ivec4(0)));
        CORE_ASERT(!full, "Fuond more than 4 bones pre vertex, check the import settings");
        for (int k = 0; k < max_vtx_bones; k++) {
          if (vertex.bone_id[k] < 0) {
            vertex.bone_id[k] = node.bid;
            vertex.bone_wt[k] = weight;
            break;
          }
        }
      }
    }
  }

  auto& mesh = meshes.emplace_back(vertices, indices);

  aiMaterial* ai_material = ai_root->mMaterials[ai_mesh->mMaterialIndex];
  process_material(ai_material, mesh);

  // 统计mesh数
  meshes_name.push_back(ai_mesh->mName.C_Str());
  n_meshes++;
}

void Model::process_material(aiMaterial* ai_material, const Mesh& mesh) {
  CORE_ASERT(ai_material != nullptr, "material is nullptr");
  aiString name;
  if (ai_material->Get(AI_MATKEY_NAME, name) != AI_SUCCESS) {
    CORE_ERROR("unable to load mesh's material (vao = {})", mesh.material_id);
  }

  std::string matkey(name.C_Str());

  // 绑定的材质存在
  if (materials_cache.find(matkey) != materials_cache.end()) {
    GLuint matid = materials_cache[matkey];
    mesh.set_material_id(matid);
    return;
  }

  GLuint matid = mesh.material_id;
  materials_cache[matkey] = matid;
}

Material& Model::SetMatermial(const std::string& matkey, const Material& material) {
  CORE_ASERT(materials_cache.count(matkey) > 0, "Invalid material key: {0}", matkey);
  GLuint matid = materials_cache[matkey];
  materials.insert_or_assign(matid, material);

  return materials.at(matid);
}

void Model::AttachMotion(const std::string& filepath) {
  if (!m_animated) {
    CORE_ERROR("Cannot attach animation to the model, model must be animated...");
    throw std::runtime_error("opt error");
  }
  animation_filepath = filepath;
  // clang-format off
  const unsigned int import_options = 0
    | aiProcess_FlipUVs
    | aiProcess_Triangulate
    | aiProcess_GenSmoothNormals
    | aiProcess_FindInvalidData
    | aiProcess_ValidateDataStructure
    | aiProcess_CalcTangentSpace
    | aiProcess_LimitBoneWeights;
  //clang-format on

  Assimp::Importer importer;
  importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
  importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_READ_ANIMATIONS, true);
  const aiScene* scene = importer.ReadFile(filepath, import_options);

  if(!scene || scene->mRootNode == nullptr) {
    CORE_ERROR("Failed to import animation: {0}",  filepath);
    CORE_ERROR("Assimp error: {0}", importer.GetErrorString());
    throw std::runtime_error("Unexpection...");
  }
  animation = std::make_unique<Animation>(scene, this);
}

}  // namespace component
