#include "core/App.h"

#include <iostream>

#include "component/Mesh.h"
#include "core/Window.h"
#include "core/Log.h"
#include "scene/Render.h"

using  Render = scene::Render ;

App& App::instand() {
  static App i;
  return i;
}

void App::init() {
  // 初始化窗口系统
  Window::init();
  core::Log::init();
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
  // 渲染器绑定场景
  Render::Attach("hellow word");
}

void App::render_update() {
  // 画面渲染
  Render::draw_scene();  
  // 双缓存 窗口回调
  Window::update();
  
}

App::App() {}

App::~App() { Window::clear(); }
