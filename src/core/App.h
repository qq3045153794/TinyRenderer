#ifndef _LEARN_OPENGL_SRC_CORE_APP_H_
#define _LEARN_OPENGL_SRC_CORE_APP_H_

#include "asset/Shader.h"
#include "asset/Texture.h"
#include "component/Mesh.h"
class App {
 public:
  static App& instand();
  void init();
  void run();
  static bool m_app_run;

 private:
  std::shared_ptr<Mesh> m_mesh;
  std::shared_ptr<asset::Shader> m_shader;
  std::shared_ptr<asset::Texture> m_texture;
  App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  ~App();
};

#endif