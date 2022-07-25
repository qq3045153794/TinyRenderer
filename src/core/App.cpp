#include <iostream>
#include "core/App.h"
#include "core/Window.h"

bool App::m_app_run = true;

App& App::instand() {
  static App i;
  return i;
}

void App::init() {
  
  Window::instand().init();
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }
}

void App::run() {
  while(m_app_run) {
    Window::instand().update();
  }
}

App::App() {

}

App::~App() {
  Window::instand().clear();
}

