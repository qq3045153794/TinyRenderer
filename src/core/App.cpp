#include "core/App.h"

#include <iostream>

#include "component/Mesh.h"
#include "core/Window.h"
#include "scene/Render.h"

using  Render = scene::Render ;

App& App::instand() {
  static App i;
  return i;
}

void App::init() {
  
  Window::init();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
  Render::Attach("hellow word");
}

void App::render_update() {
  
  Render::draw_scene();  
  Window::update();
  
}

App::App() {}

App::~App() { Window::clear(); }
