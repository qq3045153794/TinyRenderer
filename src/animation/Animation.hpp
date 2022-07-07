#ifndef _LEARN_OPENGL_SRC_ANIMATION_ANIMATION_H_
#define _LEARN_OPENGL_SRC_ANIMATION_ANIMATION_H_

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <functional>
#include <assimp/scene.h>
#include "Bone.hpp"
#include "BoneInfo.hpp"
#include "model.hpp"

namespace gl_animation {

struct AssimpNodeData {
  glm::mat4 transformation;
  std::string name;
  int children_count;
  std::vector<AssimpNodeData> children;
};

class Animation {
 public:
  Animation() = default;
  Animation(const std::string& animation_path, Model* model);
  ~Animation() { }
  Bone* find_bone(const std::string& name);
  float get_ticksper_second();
  float get_duration();
  const AssimpNodeData& get_root_node();
  const std::map<std::string, BoneInfo>& get_bone_id_map();
 private:
  void read_missing_bones(const aiAnimation* animation, Model& model);
  void read_heirarchy_data(AssimpNodeData& dest, const aiNode* src);
  
  float duration_;
  int ticksper_second_;
  std::vector<Bone> bones_;
  AssimpNodeData root_node_;
  std::map<std::string, BoneInfo> bone_info_map_; 
};

Animation::Animation(const std::string& animation_path, 
                     Model* model) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(animation_path, aiProcess_Triangulate);
  auto animation = scene->mAnimations[0];
  duration_ = animation->mDuration;
  ticksper_second_ = animation->mTicksPerSecond;
  aiMatrix4x4 global_transformation = scene->mRootNode->mTransformation;
  global_transformation = global_transformation.Inverse();
  read_heirarchy_data(root_node_, scene->mRootNode);
  read_missing_bones(animation, *model);
}

void Animation::read_missing_bones(const aiAnimation* animation, 
                                   Model& model) {
  int size = animation->mNumChannels;

  auto& bone_info_map = model.get_bone_info_map();
  int bone_count = model.getBoneCount();

  for(size_t i =0; i < size; i++) {
    auto channel = animation->mChannels[i];

    std::string bone_name = channel->mNodeName.data;

    if(bone_info_map.find(bone_name) == bone_info_map.end()) {
      bone_info_map[bone_name].id = bone_count;
      bone_count++;
    }
    bones_.push_back(Bone(channel->mNodeName.data, bone_info_map[channel->mNodeName.data].id, channel));
  }
  bone_info_map_ = bone_info_map;
}

void Animation::read_heirarchy_data(AssimpNodeData& dest, 
                                    const aiNode* src) {
  dest.name = src->mName.data;
  dest.transformation = AssimpTOGlm::convert_matrix_to_glm_format(src->mTransformation);
  dest.children_count = src->mNumChildren;
  for(size_t i = 0; i< src->mNumChildren; i++) {
    AssimpNodeData new_data;
    read_heirarchy_data(new_data, src->mChildren[i]);
    dest.children.push_back(new_data);
  }
}

Bone* Animation::find_bone(const std::string& name) {
  auto iter = std::find_if(bones_.begin(),bones_.end(),
    [&](const Bone& bone){
      return bone.get_bone_name() == name;
    });
  if(iter == bones_.end()) return nullptr;
  else return &(*iter);
}

float Animation::get_ticksper_second() {
  return ticksper_second_;
}

float Animation::get_duration() {
  return duration_;
}

const AssimpNodeData& Animation::get_root_node() {
  return root_node_;
}

const std::map<std::string, BoneInfo>& Animation::get_bone_id_map() {
  return bone_info_map_;
}

}
#endif