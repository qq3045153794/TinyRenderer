#ifndef _SRC_COMPONENT_MODEL_H_
#define _SRC_COMPONENT_MODEL_H_

// clang-format off
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
// clang-format on

namespace component {

class Mesh;

class Node {};

enum Quality {
  Auto = 0x0,
  Low = aiProcessPreset_TargetRealtime_Fast,
  Medium = aiProcessPreset_TargetRealtime_Quality,
  High = aiProcessPreset_TargetRealtime_MaxQuality
};

class Model {
 public:
  Model(const std::string& file_path, Quality quality);

  const aiScene* ai_root = nullptr;

  std::vector<Node> nodes;
  std::vector<Mesh> meshes;

 private:
  void process_tree(aiNode* ai_node, int parent);
  void process_node(aiNode* ai_node);
  void process_mesh(aiNode* ai_mesh);
};

}  // namespace component

#endif