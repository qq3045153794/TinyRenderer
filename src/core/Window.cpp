#include "core/Window.h"

Window& Window::instand() {
  static Window i;
  return i;
}

Window::Window()
  : m_width(800U),
    m_height(600U),
    m_x(0U),
    m_y(0U),
    title("LearnOpenGL") {

}

Window::~Window() {
  if(m_window) {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }
}

void Window::clear() {
  if(m_window) {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }
}

void Window::init() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);

  glfwMakeContextCurrent(m_window);
}

void Window::update() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glfwSwapBuffers(m_window);
  glfwPollEvents();
}