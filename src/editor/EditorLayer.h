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
  void NewScene(bool& open);
  void OpenScene(bool& open);
  void SaveScene(bool& dir_open, bool& name_open);
  void Reset();
  void TriggerViewPort();
  ::scene::Entity create_entity(const std::string& name, component::ETag tag = component::ETag::Untagged);
  entt::registry registry;
  std::shared_ptr<scene::Scene> m_cur_scene;
  std::shared_ptr<scene::Scene> m_new_scene;
  ::scene::Entity m_editor_camera;
  // std::shared_ptr<::asset::FBO> main_fbo;
  std::unique_ptr<SceneHierarchyPanel> m_hierarchy_panel{nullptr};
  std::unique_ptr<ContentBrowerPanel> m_content_brower_panel{nullptr};
  // 场景区域 左上 右下两点
  glm::vec2 bound_viewport[2];
  std::string m_cur_scene_name{""};
  std::optional<std::filesystem::path> m_save_file_name_path;
  // 创建entity 例如 editor camera等
};
}  // namespace editor
}  // namespace saber
#endif
