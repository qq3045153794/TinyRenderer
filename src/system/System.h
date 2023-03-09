#ifndef _SRC_SYSTEM_SYSTEM_
#define _SRC_SYSTEM_SYSTEM_
#include <scene/Scene.h>
namespace saber {
namespace system {
class System {
 public:
  System() = default;
  System(std::shared_ptr<scene::Scene> scene) : m_scene(scene) {}
  virtual ~System() = default;
  virtual void onUpdateRuntime() = 0;
  virtual void onEditorRumtime() = 0;

 private:
  std::shared_ptr<scene::Scene> m_scene;
};

}  // namespace system
}  // namespace saber
#endif
