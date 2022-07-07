#ifndef _LEARN_OPENGL_SRC_ANIMATION_ANIMATOR_H_
#define _LEARN_OPENGL_SRC_ANIMATION_ANIMATOR_H_

#include <map>
#include <vector>

#include "Animation.hpp"
#include "Bone.hpp"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "glm/glm.hpp"

namespace gl_animation {

class Animator {
 public:
  Animator(Animation* animation);
  void update_animation(float dt);
  void play_animation(Animation* p_animation);
  void calculate_bone_transform(const AssimpNodeData* node, 
                                glm::mat4 parent_trans_form);
  std::vector<glm::mat4> get_final_bone_matrices();
 private:
  std::vector<glm::mat4> final_bone_matrices_;
  Animation* current_animation_;
  float current_time_;
  float delta_time_;
};

Animator::Animator(Animation* animation) {
  current_time_ = 0.0;
  current_animation_ = animation;

  final_bone_matrices_.reserve(100);

  for(size_t i = 0; i < 100; i++) {
    final_bone_matrices_.push_back(glm::mat4(1.0));
  }
}

void Animator::update_animation(float dt) {
  delta_time_ = dt;
  if(current_animation_) {
    current_time_ += dt * current_animation_->get_ticksper_second();
    current_time_ = fmod(current_time_, current_animation_->get_duration());
    calculate_bone_transform(&current_animation_->get_root_node(), glm::mat4(1.0f));
  }
}

void Animator::play_animation(Animation* p_animation) {
  current_animation_ = p_animation;
  current_time_ = 0.0f;
}

void Animator::calculate_bone_transform(const AssimpNodeData* node, 
                                        glm::mat4 parent_trans_form) {
  std::string node_name = node->name;
  glm::mat4 node_transform = node->transformation;
  Bone* bone = current_animation_->find_bone(node_name);
  if(bone) {
    bone->update(current_time_);
    node_transform = bone->get_local_transform();
  }
  glm::mat4 global_transformation = parent_trans_form * node_transform;

  auto bone_info_map = current_animation_->get_bone_id_map();
  if(bone_info_map.find(node_name) != bone_info_map.end()) {
    int index = bone_info_map[node_name].id;
    glm::mat4 offset = bone_info_map[node_name].offset;
    
    final_bone_matrices_[index] = global_transformation * offset;
  }
  
  for(size_t i = 0; i < node->children_count; i++) {
    calculate_bone_transform(&node->children[i], global_transformation);
  }

}

std::vector<glm::mat4> Animator::get_final_bone_matrices() {
  return final_bone_matrices_;
}

}
#endif