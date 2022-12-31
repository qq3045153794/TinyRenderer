/*
 * @Author       : xietao
 * @Date         : 2022-12-29 14:09:16
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-31 17:31:41
 * @FilePath     : /TinyRenderer/src/scene/Scene.h
 * @Description  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _SRC_SCENE_SCENE_H_
#define _SRC_SCENE_SCENE_H_

#include <string>

#include "asset/FBO.h"
#include "component/Tag.hpp"
#include "imgui.h"
#include "scene/Entity.hpp"

namespace scene {

class Scene {
  friend class Render;

 public:
  std::string m_title;
  Scene(const std::string& title);
  virtual ~Scene();
  Entity create_entity(const std::string& name, component::ETag tag = component::ETag::Untagged);
  void add_nor_ubo();
  void set_nor_ubo(GLuint uid, std::shared_ptr<asset::Shader> shader);

  void add_fbo(GLuint width, GLuint height);

  virtual void init();
  virtual void on_scene_render();
  virtual void on_imgui_render();

 protected:
  // 相机 UBO
  std::shared_ptr<asset::UBO> nor_ubo;
  // 后期处理
  std::shared_ptr<asset::FBO> nor_fbo;

  entt::registry registry;
  std::map<entt::entity, std::string> directory;

 private:
  ImTextureID welcome_screen_texture_id;
  std::unique_ptr<asset::Texture> welcome_screen;
};

}  // namespace scene

#endif