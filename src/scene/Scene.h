#ifndef _LEARN_OPENGL_SRC_SCENE_SCENE_H_
#define _LEARN_OPENGL_SRC_SCENE_SCENE_H_

#include <string>

#include "scene/Entity.hpp"
#include "asset/FBO.h"

namespace scene {

class Scene {
  friend class Render;

 public:
  std::string m_title;
  Scene(const std::string& title);
  virtual ~Scene();
  Entity create_entity(const std::string& name);
  void add_nor_ubo();
  void set_nor_ubo(GLuint uid, std::shared_ptr<asset::Shader> shader);

  void add_fbo(GLuint width, GLuint height);

  virtual void init();
  virtual void on_scene_render();

 protected:
  // 相机 UBO
  std::shared_ptr<asset::UBO> nor_ubo;
  // 后期处理
  std::shared_ptr<asset::FBO> nor_fbo;
  entt::registry registry;
  std::map<entt::entity, std::string> directory;
};

}  // namespace scene

#endif