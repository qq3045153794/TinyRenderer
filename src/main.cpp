
#include "core/App.h"
#include "core/Log.h"

int main() {
  auto& app = core::App::instand();
  app.init();

  std::cout << "$ System Information" << '\n' << std::endl;
  std::cout << "  GPU Vendor Name:   " << app.gl_vendor << std::endl;
  std::cout << "  OpenGL Renderer:   " << app.gl_renderer << std::endl;
  std::cout << "  OpenGL Version:    " << app.gl_version << std::endl;
  std::cout << "  GLSL Core Version: " << app.glsl_version << '\n' << std::endl;

  std::cout << "$ Maximum supported texture size: " << '\n' << std::endl;
  std::cout << "  1D / 2D texture (width and height): " << app.gl_texsize
            << std::endl;
  std::cout << "  3D texture (width, height & depth): " << app.gl_texsize_3d
            << std::endl;
  std::cout << "  Cubemap texture (width and height): "
            << app.gl_texsize_cubemap << std::endl;
  std::cout << "  Max number of texture units: " << app.gl_max_texture_units
            << '\n'
            << std::endl;

  std::cout << "$ Maximum allowed number of uniform buffers: " << '\n'
            << std::endl;
  std::cout << "  Vertex shader:   " << app.gl_maxv_ubos << std::endl;
  std::cout << "  Geometry shader: " << app.gl_maxg_ubos << std::endl;
  std::cout << "  Fragment shader: " << app.gl_maxf_ubos << std::endl;

  while (app.run()) {
    core::App::instand().render_update();
    core::App::instand().event_update();
  }
  app.clear();
}
