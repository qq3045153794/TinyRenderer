#ifndef _SRC_SYSTEM_SYSTEM_
#define _SRC_SYSTEM_SYSTEM_

namespace scene {
class Scene;
class Entity;
}

namespace saber {
namespace system {

class System {
 public:
  System() = default;
  System(scene::Scene* scene);
  virtual ~System() = default;
  // 使用时调用
  virtual void Awake() = 0;
  virtual void OnUpdateRuntime() = 0;
  virtual void OnEditorRumtime(scene::Entity editor_camera) = 0;

 protected:
  scene::Scene* m_scene{nullptr};
};

}  // namespace system
}  // namespace saber
#endif
