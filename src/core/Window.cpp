#include "core/Window.h"


Window::Window() {}

Window::~Window() {
  if (m_window) {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }
}

void Window::clear() {
  if (m_window) {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }
}

void Window::init() {
  m_width = 800U;
  m_height = 600U;
  m_x = m_y = 0U;
  title = "LearnOpenGL";
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);

  glfwMakeContextCurrent(m_window);
}

void Window::update() {
  glfwSwapBuffers(m_window);
  glfwPollEvents();
}

void Window::clear_buffer() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::rename(const std::string& title) { glfwSetWindowTitle(m_window, title.c_str()); }