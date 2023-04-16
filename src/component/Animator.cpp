#include <component/Animator.h>
#include <utils/math.h>

namespace component {

static const glm::mat4 identity_m = glm::mat4(1.F);
static const glm::quat identity_q = glm::quat(1.F, 0.F, 0.F, 0.F);

Channel::Channel(aiNodeAnim* ai_channel, const std::string& name, int id, float duration) : name(name), bone_id(id) {
  unsigned int n_positions = ai_channel->mNumPositionKeys;
  unsigned int n_rotations = ai_channel->mNumRotationKeys;
  unsigned int n_scales = ai_channel->mNumScalingKeys;

  if (n_positions < 1 || n_rotations < 1 || n_scales < 1) {
    throw std::runtime_error("Invalid animation channel, require at least one frame pre key...");
  }
  positions.resize(n_positions + 1, FT(glm::vec3(0.0F), -1.0F));
  rotations.resize(n_rotations + 1, FR(identity_q, -1.F));
  scales.resize(n_scales + 1, FS(glm::vec3(1.F), -1.F));

  for (auto [i, prev_time] = std::tuple(0U, 0.F); i < n_positions; i++) {
    auto& frame = ai_channel->mPositionKeys[i];
    auto& value = frame.mValue;
    auto timestamp = static_cast<float>(frame.mTime);

    CORE_ASERT(timestamp >= prev_time, "Assimp failed to return frames in chronological order!");
    positions[i].value = glm::vec3(value.x, value.y, value.z);
    positions[i].timestamp = timestamp;
    prev_time = timestamp;
    if (i == 0) {
      auto& last_frame = positions.back();
      last_frame.value = glm::vec3(value.x, value.y, value.z);
      last_frame.timestamp = duration;
    }
  }

  for (auto [i, prev_time] = std::tuple(0U, 0.F); i < n_rotations; i++) {
    auto& frame = ai_channel->mRotationKeys[i];
    auto& value = frame.mValue;
    auto timestamp = static_cast<float>(frame.mTime);
    CORE_ASERT(timestamp >= prev_time, "Assimp failed to retrun frames in chronological order!");
    rotations[i].value = glm::quat(value.w, value.x, value.y, value.z);
    rotations[i].timestamp = timestamp;
    prev_time = timestamp;
    if (i == 0) {
      auto& last_frame = rotations.back();
      last_frame.value = glm::quat(value.w, value.x, value.y, value.z);
      last_frame.timestamp = duration;
    }
  }

  for (auto [i, prev_time] = std::tuple(0U, 0.F); i < n_scales; i++) {
    auto& frame = ai_channel->mScalingKeys[i];
    auto& value = frame.mValue;
    auto timestamp = static_cast<float>(frame.mTime);

    CORE_ASERT(timestamp >= prev_time, "Assimp failed to return frames in chronological order!");
    scales[i].value = glm::vec3(value.x, value.y, value.z);
    scales[i].timestamp = timestamp;
    prev_time = timestamp;

    if (i == 0) {
      auto& last_frame = scales.back();
      last_frame.value = glm::vec3(value.x, value.y, value.z);
      last_frame.timestamp = duration;
    }
  }
}

template <typename TFrame>
std::tuple<int, int> Channel::GetFrameIndex(const std::vector<TFrame>& frames, float time) const {
  if (frames.size() == 1) {
    return std::tuple(0, 0);
  }

  for (std::size_t i = 1U; i < frames.size(); i++) {
    auto& [_, timestamp] = frames.at(i);
    if (time < timestamp) {
      return std::tuple(i - 1, i);
    }
  }
  return std::tuple(-1, -1);
}

glm::mat4 Channel::Interpolate(float time) const {
  auto [i1, i2] = GetFrameIndex<FT>(positions, time);
  auto& [prev_position, prev_ts_t] = positions[i1];
  auto& [next_position, next_ts_t] = positions[i2];

  auto [j1, j2] = GetFrameIndex<FR>(rotations, time);
  auto& [prev_rotation, prev_ts_r] = rotations[j1];
  auto& [next_rotation, next_ts_r] = rotations[j2];

  auto [k1, k2] = GetFrameIndex<FS>(scales, time);
  auto& [prev_scale, prev_ts_s] = scales[k1];
  auto& [next_scale, next_ts_s] = scales[k2];

  // TODO
  // float percent_t = math::LinearPercent(prev_ts_t, next_ts_t, time);
  // using math = utils::math;
  // utils::math
  float percent_t = utils::math::LinearPercent(prev_ts_t, next_ts_t, time);
  float percent_r = utils::math::LinearPercent(prev_ts_r, next_ts_r, time);
  float percent_s = utils::math::LinearPercent(prev_ts_s, next_ts_s, time);

  glm::vec3 new_position = utils::math::Lerp(prev_position, next_position, percent_t);
  glm::quat new_rotation = utils::math::Slerp(prev_rotation, next_rotation, percent_r);
  glm::vec3 new_scale = utils::math::Lerp(prev_scale, next_scale, percent_s);
  glm::mat4 translation = glm::translate(identity_m, new_position);
  glm::mat4 rotation = glm::toMat4(new_rotation);
  glm::mat4 scale = glm::scale(identity_m, new_scale);

  return translation * rotation * scale;
}

Animation::Animation(const aiScene* ai_scene, Model* model) : n_channels(0) {
  CORE_ASERT(ai_scene->mNumAnimations > 0, "The input file dose not cotain animations!");

  aiAnimation* ai_animation = ai_scene->mAnimations[0];

  m_name = ai_animation->mName.C_Str();
  m_duration = static_cast<float>(ai_animation->mDuration);
  m_speed = static_cast<float>(ai_animation->mTicksPerSecond);
  channels.resize(model->n_bones);
  auto& nodes = model->nodes;

  for (std::size_t i = 0; i < ai_animation->mNumChannels; i++) {
    aiNodeAnim* ai_channel = ai_animation->mChannels[i];
    std::string bone_name = ai_channel->mNodeName.C_Str();
    auto node =
        std::find_if(nodes.begin(), nodes.end(), [&bone_name](const Node& node) { return node.name == bone_name; });

    if (node == nodes.end()) {
      continue;
    }

    if (!node->is_bone()) {
      continue;
    }

    Channel& channel = channels[node->bid];
    CORE_ASERT(channel.bone_id < 0, "This channel is already filled, duplicate bone!");
    channel = std::move(Channel(ai_channel, bone_name, node->bid, m_duration));
    nodes[node->nid].alive = true;
    n_channels++;
  }

  unsigned int cnt = std::count_if(channels.begin(), channels.end(), [](const Channel& c){ return c.bone_id >= 0; });
  CORE_ASERT(n_channels == cnt, "Incorrect channels count, must match bones 1 on 1!");
  CORE_ASERT(n_channels <= model->n_bones, "Invalid channels are not dropped, please clean up!");
}

Animator::Animator(Model* model) { Reset(model); }

void Animator::Reset(Model* model) {
  CORE_ASERT(model->m_animated, "Model doesn't have animation!");
  m_bone_transforms.resize(model->n_bones, identity_m);
  m_current_time = 0.F;
}

void Animator::Update(Model& model, float deltatime) {
  const auto& animation = model.animation;
  m_current_time += animation->m_speed * deltatime;
  m_current_time = fmod(m_current_time, animation->m_duration);
  const auto& channels = animation->channels;
  auto& nodes = model.nodes;

  for (auto& node : nodes) {
    int& bone_id = node.bid;
    int& parent_id = node.pid;
    glm::mat4 n2p = node.animated() ? channels[bone_id].Interpolate(m_current_time) : node.n2p;
    glm::mat4 p2m = parent_id < 0 ? glm::inverse(nodes[0].n2p) : nodes[parent_id].n2m;
    node.n2m = p2m * n2p;
    if (node.is_bone()) {
      m_bone_transforms[bone_id] = node.n2m * node.m2n;
    }
  }
}

}  // namespace component
