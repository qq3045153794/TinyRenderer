// clang-format off
#include "core/App.h"

#include <iostream>

#include "component/Mesh.h"
#include "core/Window.h"
#include "core/Log.h"
#include "core/Clock.h"
#include "core/Input.h"
#include "core/Event.h"
#include "scene/RenderCommand.h"
#include "scene/ui.h"
// clang-format on

namespace core {

using RenderCommand = scene::RenderCommand;

App& App::instand() {
  static App i;
  return i;
}

void App::init() {
  // 初始化窗口系统
  Window::init();
  Log::init();
  Clock::init();
  Input::init();
  Event::register_callback();
  scene::ui::init();
  CORE_INFO("scene ui init success");

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    CORE_ERROR("Failed to initialize GLAD");
    return;
  }

  gl_vendor =
      std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  gl_renderer =
      std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  gl_version =
      std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  glsl_version = std::string(
      reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

  // texture size limit, max number of texture units and image units
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gl_texsize);
  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &gl_texsize_3d);
  glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &gl_texsize_cubemap);
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &gl_max_texture_units);

  // max number of uniform blocks in each shader stage
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &gl_maxv_ubos);
  glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &gl_maxg_ubos);
  glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &gl_maxf_ubos);

  // 渲染器绑定场景
  // Render::attach("Welcome Screen");
#if 1
  Window::rename("title");
  Input::clear();
  editor_layer = std::make_shared<::saber::editor::EditorLayer>();
  editor_layer->OnAttach();
  editor_layer->Awake();
#endif
}

void App::clear() {
  Window::clear();
  RenderCommand::detach();
  scene::ui::show_down();
}

bool App::run() { return !Window::should_close(); }

void App::render_update() {
  // 画面渲染
  RenderCommand::draw_scene();
}

void App::event_update() {
  if (Input::get_key_down('\r')) {
    // 切换layer
    Window::on_layer_switch();
    Input::set_key_down('\r', false);
  }

  Clock::update();
  RenderCommand::draw_imGui();
}

void App::Run() {
  editor_layer->OnUpdateRuntime();
  editor_layer->OnImGuiRender();
  Window::update();
  Clock::update();
}

App::App() {}

App::~App() {}
}  // namespace core
