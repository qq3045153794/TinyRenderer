#ifndef _LEARN_OPENGL_SRC_CORE_APP_H_
#define _LEARN_OPENGL_SRC_CORE_APP_H_

#include "asset/Shader.h"
#include "asset/Texture.h"
#include "component/Mesh.h"
class App {
 public:
  static App& instand();
  void init();
  void render_update();

 private:
  App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  ~App();
};

#endif