#ifndef _LEARN_OPENGL_SRC_SCENE_SCENE_H_
#define _LEARN_OPENGL_SRC_SCENE_SCENE_H_

#include <string>

#include "scene/Entity.hpp"

namespace scene {

class Scene {
  friend class Render;

 public:
  std::string m_title;
  Scene(const std::string& title);
  virtual ~Scene();
  Entity create_entity(const std::string& name);
  virtual void init();
  virtual void on_scene_render();

 private:
  entt::registry registry;
  std::map<entt::entity, std::string> directory;
};

}  // namespace scene

#endif