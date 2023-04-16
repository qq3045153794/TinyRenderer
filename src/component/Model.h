/*
 * @Author       : xietao
 * @Date         : 2022-12-29 14:36:02
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-30 19:45:51
 * @FilePath     : /Learn_OpenGL/src/component/Model.h
 * @Description  : 加载模型文件
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _SRC_COMPONENT_MODEL_H_
#define _SRC_COMPONENT_MODEL_H_

// clang-format off
#include <unordered_map>
#include <map>
#include <vector>
#include <bitset>
#include <memory>

#include "component/Material.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include "glad/glad.h"
// clang-format on

namespace component {

class Mesh;
class Animation;
// class Material;

class Node {
 public:
  Node(int nid, int pid, const std::string& name);
  bool is_bone() const;
  bool animated() const;
  int nid = -1;
  int pid = -1;
  int bid = -1;
  bool alive = false;
  std::string name;
  glm::mat4 n2p;
  glm::mat4 m2n;
  glm::mat4 n2m;
};

enum Quality {
  Auto = 0x0,
  Low = aiProcessPreset_TargetRealtime_Fast,
  Medium = aiProcessPreset_TargetRealtime_Quality,
  High = aiProcessPreset_TargetRealtime_MaxQuality
};

class Model {
 public:
  /**
   * @brief       : 构造函数
   * @param        {string&} file_path 模型文件路径
   * @param        {Quality} quality 加载模型质量 Auto Low Medium High
   * @param        {bool} m_animated 是否加载动画
   * @return       {*}
   */
  Model(const std::string& file_path, Quality quality, bool m_animated = false);
  Model(Model&& rhs);
  Model& operator=(Model&& rhs);
  ~Model();
  /**
   * @brief       : 绑定模型材质
   * @param        {string&} matkey 绑定的材质名 注意:
   * 需要和模型文件绑定的材质名相同 否则出错
   * @param        {shared_ptr<Material>} material
   * @return       {*}
   */
  Material& SetMatermial(const std::string& matkey, const Material& material);

  void AttachMotion(const std::string& filepath);

  const aiScene* ai_root = nullptr;

  std::bitset<6> vtx_format;

  std::vector<Node> nodes;
  std::vector<Mesh> meshes;

  std::vector<std::string> meshes_name;
  // mesh的材质id与材质的映射
  std::map<GLuint, Material> materials;

  // 模型纹理名称与模型mesh的id的映射
  std::unordered_map<std::string, GLuint> materials_cache;

  bool m_animated;
  unsigned int n_nodes = 0, n_bones = 0;
  unsigned int n_meshes = 0, n_verts = 0, n_tris = 0;
  std::unique_ptr<Animation> animation;

 private:
  void process_tree(aiNode* ai_node, int parent);
  void process_node(aiNode* ai_node);
  void process_mesh(aiMesh* ai_mesh);
  void process_material(aiMaterial* ai_material, const Mesh& mesh);

};

}  // namespace component

#endif
