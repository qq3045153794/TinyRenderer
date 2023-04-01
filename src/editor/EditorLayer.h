#ifndef _SRC_EDITOR_EDITOR_LAYER_H_
#define _SRC_EDITOR_EDITOR_LAYER_H_
// clang-format off
#include <scene/Scene.h>
#include <editor/SceneHierarchyPanel.h>
#include <editor/ContentBrowerPanel.h>
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
  void TriggerViewPort();
  std::shared_ptr<scene::Scene> m_cur_scene;
  ::scene::Entity m_editor_camera;
  std::shared_ptr<::asset::FBO> main_fbo;
  std::unique_ptr<SceneHierarchyPanel> m_hierarchy_panel{nullptr};
  std::unique_ptr<ContentBrowerPanel> m_content_brower_panel{nullptr};
  // 场景区域 左上 右下两点
  glm::vec2 bound_viewport[2];
  // 创建entity 例如 editor camera等
};
}  // namespace editor
}  // namespace saber
#endif
