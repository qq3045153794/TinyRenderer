#ifndef _LEARN_OPENGL_SRC_CORE_WINDOW_H_
#define _LEARN_OPENGL_SRC_CORE_WINDOW_H_

#include <string>

#include "GLFW/glfw3.h"
#include "glad/glad.h"

class Window {
 public:
  Window& instand();
  void init();
  void clear();
  GLuint m_width, m_height;
  GLuint m_x, m_y;
  std::string title;
  GLFWwindow* m_window = 0;

 private:
  Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  ~Window();
};

#endif