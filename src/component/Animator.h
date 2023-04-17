#ifndef _SRC_COMPONENT_ANIMATOR_H_
#define _SRC_COMPONENT_ANIMATOR_H_
#include <component/Model.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>
namespace component {
class Channel {
 private:
  template <typename TKey, int key>
  struct Frame {
    Frame(TKey value, float time) : value(value), timestamp(time) {}

    TKey value;
    float timestamp;
  };

  using FT = Frame<glm::vec3, 1>;
  using FR = Frame<glm::quat, 2>;
  using FS = Frame<glm::vec3, 3>;

 private:
  std::vector<FT> positions;
  std::vector<FR> rotations;
  std::vector<FS> scales;

  template <typename TFrame>
  std::tuple<int, int> GetFrameIndex(const std::vector<TFrame>& frames, float time) const;

 public:
  std::string name;
  int bone_id = -1;

  Channel() : bone_id(-1) {}
  Channel(aiNodeAnim* ai_channel, const std::string& name, int id, float duration);
  Channel(Channel&& other) = default;
  Channel& operator=(Channel&& other) = default;

  glm::mat4 Interpolate(float time) const;
};

class Model;

class Animation {
 private:
  unsigned int n_channels;
  std::vector<Channel> channels;
  friend class Animator;

 public:
  std::string m_name;
  float m_duration;
  float m_speed;
  Animation(const aiScene* ai_scene, Model* model);
};

class Animator {
 public:
  bool m_action = true;
  float m_current_time;
  std::string m_name;
  float m_duration;
  float m_speed;
  float m_tick_speed = 1.0;
  std::vector<glm::mat4> m_bone_transforms;
  Animator(Model* model);

  void Update(Model& model, float deltatime);
  void Reset(Model* model);
};

}  // namespace component

#endif
