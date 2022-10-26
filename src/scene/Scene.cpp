
#include "scene/Scene.h"

#include "scene/Render.h"

namespace scene {

Scene::Scene(const std::string& title) { m_title = title; }

Scene::~Scene() {}

void Scene::init() {

}

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
  // UBOs.emplace(uid, offset, lenght, sz);
}

void Scene::set_nor_ubo(GLuint uid, std::shared_ptr<asset::Shader> shader) {
  nor_ubo->set_binding(0, "Matrices", shader->get_id());
}

void Scene::on_scene_render() { Render::clear_buffer(); }

}  // namespace scene
