#ifndef _SRC_SYSTEM_SYSTEM_
#define _SRC_SYSTEM_SYSTEM_
namespace saber {
namespace system {

namespace scene {
class Scene;
}

class System {
 public:
  System() = default;
  System(scene::Scene* scene);
  virtual ~System() = default;

  virtual void OnUpdateRuntime() = 0;
  virtual void OnEditorRumtime() = 0;

 protected:
  scene::Scene* m_scene{nullptr};
};

}  // namespace system
}  // namespace saber
#endif
