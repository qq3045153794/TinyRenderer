#ifndef _SRC_EDITOR_EDITOR_LAYER_H_
#define _SRC_EDITOR_EDITOR_LAYER_H_
// clang-format off
#include <scene/Scene.h>
// clang-format on
namespace saber {
namespace editor {
class EditorLayer {
 public:
  EditorLayer();
  ~EditorLayer() = default;
  void OnAttach();
  void OnDetach();
  void OnUpdateRuntime();
  void OnImGuiRender();
  void OnEvent();
  void CreateScene();
 private:
  std::shared_ptr<scene::Scene> m_cur_scene;
};
}  // namespace editor
}  // namespace saber
#endif
