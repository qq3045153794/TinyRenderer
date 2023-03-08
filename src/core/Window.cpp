#include "core/Window.h"

#include "core/Input.h"
#include "core/Log.h"
namespace core {

Window::Window() {}

Window::~Window() {}

void Window::clear() {
  if (m_window) {
    glfwDestroyWindow(m_window);
    glfwTerminate();
  }
}

void Window::init() {
  m_width = 1600U;
  m_height = 900U;
  m_x = m_y = 0U;
  title = "LearnOpenGL";
  if (!glfwInit()) {
    CORE_ERROR("GLFW init failed...");
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 开启 MSAA
  glfwWindowHint(GLFW_SAMPLES, 4);

  m_window = glfwCreateWindow(m_width, m_height, title.c_str(), NULL, NULL);

  if (m_window == NULL) {
    CORE_ERROR("Create window failed...");
    glfwTerminate();
    exit(-1);
  }

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

void Window::rename(const std::string& title) {
  glfwSetWindowTitle(m_window, title.c_str());
}

void Window::resize() {
  glfwSetWindowPos(m_window, m_x, m_y);
  glfwSetWindowSize(m_window, m_width, m_height);
  glfwSetWindowAspectRatio(m_window, 16, 9);

  // 渲染fame_buffer显示大小
  glViewport(0, 0, m_width, m_height);
}
bool Window::should_close() { glfwWindowShouldClose(m_window); }
void Window::on_layer_switch() {
  layer = (layer == Layer::ImGui) ? Layer::Scene : Layer::ImGui;
  if (layer == Layer::ImGui) {
    // Input::ShowCursor();
  } else {
    // 重置输入
    Input::clear();
  }
}

}  // namespace core
