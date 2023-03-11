// clang-format off
#include <system/RenderSystem.h>
#include <scene/Scene.h>
#include <scene/Render.h>
// clang-format on
namespace saber {
namespace system {

using Texture = asset::Texture;
using FBO = ::asset::FBO;
using VAO = ::asset::VAO;
using VBO = ::asset::VBO;
using Render = ::scene::Render;
using Entity = ::scene::Entity;

RenderSystem::RenderSystem(scene::Scene* scene) : System(scene) {}

void RenderSystem::Awake() {}

void RenderSystem::OnUpdateRuntime() {
  // TODO
}

void RenderSystem::OnEditorRumtime(scene::Entity& editor_camera) {
  // TODO
  using namespace ::component;
  auto& reg = m_scene->registry;
  auto mesh_group = reg.group<Mesh>(entt::get<Transform, Material, Tag>);
  auto model_group = reg.group<Model>(entt::get<Transform, Tag>);
  auto& render_queue = m_scene->render_queue;

  for (auto& e : render_queue) {
    if (mesh_group.contains(e)) {
      auto& mesh = mesh_group.get<Mesh>(e);
      auto& transform = mesh_group.get<Transform>(e);
      auto& material = mesh_group.get<Material>(e);
      auto& tag = mesh_group.get<Tag>(e);
      CORE_DEBUG("render mesh");
      material.bind();
      material.set_uniform("model", transform.get_transform());
      if (!tag.contains(ETag::Skybox)) {
        mesh.draw();
      }
    } else if (model_group.contains(e)) {
      auto& model = model_group.get<Model>(e);
      auto& transform = model_group.get<Transform>(e);
      for (auto& mesh : model.meshes) {
        auto& material = model.materials.at(mesh.material_id);
        material.bind();
        material.set_uniform("model", transform.get_transform());
        mesh.draw();
      }
    }
  }
}

}  // namespace system
}  // namespace saber
