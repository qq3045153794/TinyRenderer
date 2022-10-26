#ifndef _LEARN_OPENGL_SRC_CORE_WINDOW_H_
#define _LEARN_OPENGL_SRC_CORE_WINDOW_H_

#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"


class Window {
 public:
  // static Window& instand();
  static void init();
  static void update();
  static void clear_buffer();
  static void clear();
  static void rename(const std::string& title);
  static inline GLuint m_width = 0, m_height = 0;
  static inline GLuint m_x = 0 , m_y = 0;
  static inline std::string title = "";
  static inline GLFWwindow* m_window = 0;

 private:
  Window();
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  ~Window();
};

#endif