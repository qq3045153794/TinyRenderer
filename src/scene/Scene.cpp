
#include "scene/Scene.h"

#include "core/Log.h"
#include "scene/Render.h"

namespace scene {

Scene::Scene(const std::string& title) { m_title = title; }

Scene::~Scene() {}

void Scene::init() {}

Entity Scene::create_entity(const std::string& name) {
  Entity e = {name, registry.create(), &registry};
  e.AddComponent<component::Transform>();
  directory.emplace(e.id, e.name);
  return e;
}

void Scene::add_nor_ubo() {
  std::vector<GLuint> offset = {0U, 64U};
  std::vector<GLuint> lenght = {64U, 64U};
  GLuint sz = 128U;

  nor_ubo = std::make_shared<asset::UBO>(offset, lenght, sz);
}

void Scene::set_nor_ubo(GLuint uid, std::shared_ptr<asset::Shader> shader) {
  nor_ubo->set_binding(0, "Matrices", shader->get_id());
}

void Scene::add_fbo(GLuint width, GLuint height) {
  nor_fbo = std::make_shared<asset::FBO>(width, height);
  nor_fbo->set_color_texture();
  nor_fbo->set_depth_texture();
  CORE_INFO("add FBO widht :{} height: {}", width, height);
}

void Scene::on_scene_render() { Render::clear_buffer(); }

void Scene::on_imgui_render() {}

}  // namespace scene
