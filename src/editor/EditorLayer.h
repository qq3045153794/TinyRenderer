#ifndef _SRC_EDITOR_EDITOR_LAYER_H_
#define _SRC_EDITOR_EDITOR_LAYER_H_
// clang-format off
#include <scene/Scene.h>
#include <system/System.h>
#include <system/RenderSystem.h>
#include <system/EnvironmentSystem.h>
// clang-format on
namespace saber {
namespace editor {
class EditorLayer {
 public:
  void onUpdate();
  void onImGUIUpdate();
  void CreateScene();

 private:
  std::shared_ptr<scene::Scene> m_cur_scene;
  std::shared_ptr<system::System> m_render_system;
  std::shared_ptr<system::System> m_environment_system;
};
}  // namespace editor
}  // namespace saber
#endif
