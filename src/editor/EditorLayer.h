#ifndef _SRC_EDITOR_EDITOR_LAYER_H_
#define _SRC_EDITOR_EDITOR_LAYER_H_
// clang-format off
#include <scene/Scene.h>
#include <editor/SceneHierarchyPanel.h>
// clang-format on
namespace saber {
namespace editor {
class EditorLayer {
 public:
  EditorLayer();
  ~EditorLayer() = default;
  void Awake();
  void OnAttach();
  void OnDetach();
  void OnUpdateRuntime();
  void OnImGuiRender();
  void OnEvent();
  void CreateScene();
 private:
  void NewScene();
  void OpenScene();
  void SaveScene();
  std::shared_ptr<scene::Scene> m_cur_scene;
  ::scene::Entity m_editor_camera;
  std::shared_ptr<::asset::FBO> main_fbo;
  std::unique_ptr<SceneHierarchyPanel> m_hierarchy_panel{nullptr};
};
}  // namespace editor
}  // namespace saber
#endif
