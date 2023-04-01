/*
 * @Author       : xietao
 * @Date         : 2022-12-29 14:09:16
 * @LastEditors  : xietao
 * @LastEditTime : 2023-01-06 15:25:17
 * @FilePath     : /TinyRenderer/src/scene/Scene.h
 * @Description  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _SRC_SCENE_SCENE_H_
#define _SRC_SCENE_SCENE_H_

#include <string>
#include <set>

#include "asset/FBO.h"
#include "component/Tag.hpp"
#include "scene/Entity.hpp"
#include "system/System.h"
#include "system/EnvironmentSystem.h"
#include "system/RenderSystem.h"

namespace scene {

class Scene {
  friend class RenderCommand;

  friend class ::saber::system::RenderSystem;
  friend class ::saber::system::EnvironmentSystem;
 public:
  std::string m_title;
  Scene(const std::string& title);
  virtual ~Scene();
  Entity create_entity(const std::string& name,
                       component::ETag tag = component::ETag::Untagged);


  void registry_shader(GLuint shader_id);
  void exclude_entity(const entt::entity& id);
  bool is_exclude_entity(const entt::entity& id);

  template <typename... Args>
  void SubmitRender(Args&&... args) {
    (render_queue.push_back(args), ...);
  }

  virtual void Awake();
  virtual void OnUpdateRuntime();
  virtual void OnEditorRumtime(::scene::Entity& editor_camera);

  entt::registry registry;
 protected:

  std::map<entt::entity, std::string> directory;
  std::set<entt::entity> exclude_entitys;
  std::vector<entt::entity> render_queue;
  std::set<uint32_t> shader_id_set;
  std::vector<std::shared_ptr<saber::system::System>> m_systems;

 private:
};

}  // namespace scene

#endif
