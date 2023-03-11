#ifndef _SRC_SYSTEM_RENDER_SYSTEM_
#define _SRC_SYSTEM_RENDER_SYSTEM_
#include <system/System.h>
namespace saber {
namespace system {

class RenderSystem : public System {
public:
  RenderSystem() = default;
  RenderSystem(scene::Scene* scene);
  virtual ~RenderSystem() = default;

  virtual void OnUpdateRuntime() override;
  virtual void OnEditorRumtime(scene::Entity editor_camera) override;
};

}  // namespace system
}  // namespace saber
#endif
