#ifndef _LEARN_OPENGL_SRC_ANIMATION_BONE_H_
#define _LEARN_OPENGL_SRC_ANIMATION_BONE_H_
#include <vector>
#include <assimp/scene.h>
#include <list>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <BoneInfo.hpp>
#include "AssimToGlm.hpp"

namespace gl_animation {

struct KeyPosition {
  glm::vec3 position;
  float time_stamp;
};

struct KeyRotation {
  glm::quat orientation;
  float time_stamp;
};

struct KeyScale {
  glm::vec3 scale;
  float time_stamp;
};

class Bone {
 public:
  Bone(const std::string& name, int id, const aiNodeAnim* channel);
  void update(float animation_time);
  glm::mat4& get_local_transform();
  const std::string& get_bone_name() const { return name_;};
  const int get_bone_id() { return id_;};
  int get_position_index(float animation_time);
  int get_rotation_index(float animation_time);
  int get_scale_index(float animation_time);

 private:
  float get_scale_factor(float last_time_stamp, float next_time_stamp, float animation_time);
  glm::mat4 interpolate_position(float animation_time);
  glm::mat4 interpolate_rotation(float animation_time);
  glm::mat4 interpolate_scaling(float animation_time);

  std::vector<KeyPosition> positions_;
  std::vector<KeyRotation> rotations_;
  std::vector<KeyScale> scales_;
  int num_positions_;
  int num_rotations_;
  int num_scalings_;
  glm::mat4 local_transform_;
  std::string name_;
  int id_;
};

Bone::Bone(const std::string& name, int id, const aiNodeAnim* channel) 
  : name_(name),
    id_(id),
    local_transform_(1.0f) {
    
    num_positions_ = channel->mNumPositionKeys;

    for(int position_index = 0; position_index < num_positions_; position_index++) {
      aiVector3D ai_position = channel->mPositionKeys[position_index].mValue;
      float time_stamp = channel->mPositionKeys[position_index].mTime;
      KeyPosition data;
      data.position = AssimpTOGlm::get_glm_vec(ai_position);
      data.time_stamp = time_stamp;
      positions_.push_back(data);
    }

    num_rotations_ = channel->mNumRotationKeys;
    for(int rotation_index = 0; rotation_index < num_rotations_; ++rotation_index) {
      aiQuaternion aiOrientation = channel->mRotationKeys[rotation_index].mValue;
      float time_stamp = channel->mPositionKeys[rotation_index].mTime;
      KeyRotation data;
      data.orientation = AssimpTOGlm::get_glm_quat(aiOrientation);
      data.time_stamp = time_stamp;
      rotations_.push_back(data);
    }

    
    num_scalings_ = channel->mNumScalingKeys;
    for(int key_index = 0; key_index < num_scalings_; key_index++) {
      aiVector3D scale = channel->mScalingKeys[key_index].mValue;
      float time_stamp = channel->mScalingKeys[key_index].mTime;

      KeyScale data;
      data.scale = AssimpTOGlm::get_glm_vec(scale);
      data.time_stamp = time_stamp;
      scales_.push_back(data);
    }

}

void Bone::update(float animation_time) {
  glm::mat4 translation = interpolate_position(animation_time);
  glm::mat4 rotation = interpolate_rotation(animation_time);
  glm::mat4 scale = interpolate_scaling(animation_time);
  local_transform_ = translation * rotation * scale; 
}

glm::mat4 Bone::interpolate_position(float animation_time) {
  if(num_positions_ == 1) {
    return glm::translate(glm::mat4(1.0), positions_[0].position);
  }
  size_t p0 = get_position_index(animation_time);
  size_t p1 = p0 + 1;
  float scale_factor = get_scale_factor(positions_[p0].time_stamp, positions_[p1].time_stamp, animation_time);
  glm::vec3 final_position = glm::mix(positions_[p0].position, positions_[p1].position, scale_factor);
  return glm::translate(glm::mat4(1.0f), final_position);
}

glm::mat4 Bone::interpolate_rotation(float animation_time) {
  if(num_rotations_ == 1) {
    auto rotation = glm::normalize(rotations_[0].orientation);
    return glm::toMat4(rotation);
  }
  size_t p0 = get_rotation_index(animation_time);
  size_t p1 = p0 + 1;
  float scale_factor = get_scale_factor(rotations_[p0].time_stamp, rotations_[p1].time_stamp, animation_time);
  glm::quat final_rotation = glm::normalize(glm::slerp(rotations_[p0].orientation, rotations_[p1].orientation, scale_factor));
  return glm::toMat4(final_rotation);
}

glm::mat4 Bone::interpolate_scaling(float animation_time) {
  if(num_scalings_ == 1) {
    return glm::scale(glm::mat4(1.0), scales_[0].scale);
  }
  size_t p0 = get_scale_index(animation_time);
  size_t p1 = p0 + 1;
  float scale_factor = get_scale_factor(scales_[p0].time_stamp, scales_[p1].time_stamp, animation_time);
  glm::vec3 final_scale = glm::mix(scales_[p0].scale, scales_[p1].scale, scale_factor);
  return glm::scale(glm::mat4(1.0f), final_scale);
}

int Bone::get_position_index(float animation_time) {
  for(int i = 0; i + 1 < num_positions_; i++) {
    if(animation_time < positions_[i + 1].time_stamp){
      return i;
    }
  }
  assert(0);
}

int Bone::get_rotation_index(float animation_time) {
  for(int i = 0; i + 1 < num_rotations_; i++) {
    if(animation_time < rotations_[i + 1].time_stamp){
      return i;
    }
  }
  assert(0);
}

int Bone::get_scale_index(float animation_time){
  for(int i = 0; i + 1 < num_scalings_; i++) {
    if(animation_time < scales_[i + 1].time_stamp){
      return i;
    }
  }
  assert(0);
}

float Bone::get_scale_factor(float last_time_stamp, float next_time_stamp, float animation_time) {
  float scale_factor = 0.0f;
  float mid_way_length = animation_time - last_time_stamp;
  float frames_diff = next_time_stamp - last_time_stamp;
  scale_factor = mid_way_length / frames_diff;
  return scale_factor;
}

glm::mat4& Bone::get_local_transform() {
  return local_transform_;
}

}

#endif