#ifndef _LEARN_OPENGL_SRC_CORE_APP_H_
#define _LEARN_OPENGL_SRC_CORE_APP_H_
// clang-format off
#include "asset/Shader.h"
#include "asset/Texture.h"
#include "component/Mesh.h"
// clang-format on
namespace core {

class App {
 public:
  static App& instand();
  void init();
  void render_update();
  void event_update();

 private:
  App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  ~App();
};
}  // namespace core

#endif