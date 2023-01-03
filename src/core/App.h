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
  std::string gl_vendor, gl_renderer, gl_version, glsl_version;

  GLint gl_texsize, gl_texsize_3d, gl_texsize_cubemap;
  GLint gl_max_texture_units, gl_max_image_units;
  GLint gl_max_color_buffs;
  GLint gl_maxv_atcs, gl_maxf_atcs, gl_maxc_atcs;
  GLint gl_maxv_ubos, gl_maxg_ubos, gl_maxf_ubos, gl_maxc_ubos;
  GLint gl_maxf_ssbos, gl_maxc_ssbos;
  GLint cs_nx, cs_ny, cs_nz, cs_sx, cs_sy, cs_sz, cs_max_invocations;

 private:
  App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  ~App();
};
}  // namespace core

#endif