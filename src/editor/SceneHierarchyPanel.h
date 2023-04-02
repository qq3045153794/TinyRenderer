#ifndef _SRC_EDITOR_SCENE_HIERARCHY_PANEL_H_
#define _SRC_EDITOR_SCENE_HIERARCHY_PANEL_H_
#include <memory>
#include <scene/Entity.hpp>
namespace scene {
class Scene;
}  // namespace scene
namespace saber {
class SceneHierarchyPanel {
 public:
  SceneHierarchyPanel() = default;
  SceneHierarchyPanel(std::shared_ptr<::scene::Scene>);
  ~SceneHierarchyPanel() = default;
  void OnImGuiRender(bool* hierarchy_open);
  void ResetScene(std::shared_ptr<::scene::Scene> scene);

 private:
  void draw_components(scene::Entity& entity);
  void draw_entity_node(::scene::Entity& entity);
  std::shared_ptr<::scene::Scene> m_scene{nullptr};
  ::scene::Entity m_select_entity;
};
}  // namespace saber
#endif
