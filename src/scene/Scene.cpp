
#include "scene/Scene.h"

#include "scene/Render.h"

namespace scene {

Scene::Scene(const std::string& title) { m_title = title; }

Scene::~Scene() {}

Entity Scene::create_entity(const std::string& name) {
  Entity e = {name, registry.create(), &registry};
  e.AddComponent<component::Transform>();
  directory.emplace(e.id, e.name);
  return e;
}

void Scene::on_scene_render() { Render::clear_buffer(); }

}  // namespace scene
