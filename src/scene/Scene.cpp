
#include "scene/Scene.h"

#include "core/Log.h"
#include "scene/Render.h"
#include "scene/ui.h"

namespace scene {

Scene::Scene(const std::string& title) : m_title(title) {}

Scene::~Scene() {
  registry.each([this](auto id) {
    CORE_TRACE("Destroying entity: {0}", directory.at(id));
  });
  registry.clear();
}

void Scene::init() {
  welcome_screen = std::make_unique<asset::Texture>(
      "../resource/texture/welcome (original).png", false);
  welcome_screen_texture_id = (void*)welcome_screen->get_id();
}

Entity Scene::create_entity(const std::string& name, component::ETag tag) {
  Entity e = {name, registry.create(), &registry};
  e.AddComponent<component::Transform>();
  e.AddComponent<component::Tag>(tag);
  directory.emplace(e.id, e.name);
  m_entitys.emplace_back(e);
  return e;
}

void Scene::add_fbo(GLuint width, GLuint height) {
  nor_fbo = std::make_shared<asset::FBO>(width, height);
  nor_fbo->set_color_texture();
  nor_fbo->set_depth_texture();
}

void Scene::on_scene_render() { Render::clear_buffer(); }

void Scene::on_imgui_render() {
  ui::draw_welcome_scene(welcome_screen_texture_id);
}

}  // namespace scene
