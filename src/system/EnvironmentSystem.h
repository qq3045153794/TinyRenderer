
#ifndef _SRC_SYSTEM_ENVIRONMENT_SYSTEM_
#define _SRC_SYSTEM_ENVIRONMENT_SYSTEM_

#include <system/System.h>
namespace saber {
namespace system {
class EnvironmentSystem : public System {
public:
  EnvironmentSystem() = default;
  EnvironmentSystem(scene::Scene* scene);
  virtual ~EnvironmentSystem() = default;
  virtual void OnUpdateRuntime() override;
  virtual void OnEditorRumtime() override;
};

}  // namespace system
}  // namespace saber
#endif
