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
#include <manage/AssetManage.h>
#include <manage/ConfigManage.h>
// clang-format on

namespace core {

using RenderCommand = scene::RenderCommand;

App& App::instand() {
  static App i;
  return i;
}

void App::Init() {
  // 初始化窗口系统
  Window::init();
  Log::init();
  Clock::init();
  Input::init();
  Event::register_callback();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    CORE_ERROR("Failed to initialize GLAD");
    return;
  }

  gl_vendor = std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
  gl_renderer = std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
  gl_version = std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  glsl_version = std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

  // texture size limit, max number of texture units and image units
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gl_texsize);
  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &gl_texsize_3d);
  glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &gl_texsize_cubemap);
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &gl_max_texture_units);

  // max number of uniform blocks in each shader stage
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &gl_maxv_ubos);
  glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &gl_maxg_ubos);
  glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &gl_maxf_ubos);

  // 设置runtime 路径即资源路径
  const int MAXPATH = 255;
  char root_path[MAXPATH];
  getcwd(root_path, MAXPATH);
  this->root_path = std::string(root_path);

  ::saber::PublicSingleton<::saber::ConfigManage>::GetInstance().Init();
  ::saber::PublicSingleton<::saber::AssetManage>::GetInstance().Init();
}

void App::AttachLayer() {
  // 渲染器绑定场景
  Window::rename("title");
  Input::clear();
  editor_layer = std::make_shared<::saber::editor::EditorLayer>();
  editor_layer->OnAttach();
  editor_layer->Awake();
}

void App::Clean() {
  editor_layer.reset();
  Window::clear();
}

void App::Run() {
  while (!Window::should_close()) {
    editor_layer->OnUpdateRuntime();
    editor_layer->OnImGuiRender();
    Window::update();
    Clock::update();
  }
}

App::App() {}

App::~App() {}
}  // namespace core
