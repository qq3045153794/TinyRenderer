#ifndef _SRC_SYSTEM_RENDER_SYSTEM_
#define _SRC_SYSTEM_RENDER_SYSTEM_

#include <system/System.h>
namespace saber {
namespace system {
class RenderSystem : public System {
public:
  RenderSystem(std::shared_ptr<scene::Scene> scene);
  virtual void onUpdateRuntime() override;
  virtual void onEditorRumtime() override;
};

}  // namespace system
}  // namespace saber
#endif
