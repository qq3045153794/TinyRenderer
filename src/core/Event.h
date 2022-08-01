#ifndef _LEARN_OPENGL_SRC_CORE_H_
#define _LEARN_OPENGL_SRC_CORE_H_

#include "core/Input.h"
#include "core/Window.h"

class Event {
 public:
  Event();
  ~Event();
  static void register_callback();

 private:
  static void cursor_enter_callback(GLFWwindow* window, int entered);
  static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
  static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void window_size_callback(GLFWwindow* window, int width, int height);
};

#endif