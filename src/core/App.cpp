#include "core/App.h"

#include <iostream>

#include "component/Mesh.h"
#include "core/Window.h"
#include "core/Log.h"
#include "core/Clock.h"
#include "core/Input.h"
#include "core/Event.h"
#include "scene/Render.h"
#include "scene/ui.h"

using  Render = scene::Render ;

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
  // 渲染器绑定场景
  Render::Attach("hellow word");
}

void App::render_update() {
  // 画面渲染
  Render::draw_scene();  
  
}

void App::event_update() {
  Clock::update();
  Render::draw_imGui();

  // 双缓存 窗口回调
  Window::update();
}

App::App() {}

App::~App() { Window::clear(); }
