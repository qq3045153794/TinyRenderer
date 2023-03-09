#ifndef _SRC_EDITOR_EDITOR_LAYER_H_
#define _SRC_EDITOR_EDITOR_LAYER_H_
// clang-format off
#include <scene/Scene.h>
// clang-format on
namespace editor {
class EditorLayer {
 public:
  void onUpdate();
  void onImGUIUpdate();
  void CreateScene();

 private:
  scene::Scene* cur_scene;
};
}  // namespace editor
#endif
