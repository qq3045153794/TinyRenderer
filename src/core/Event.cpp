#include "core/Event.h"

#include <iostream>

void Event::register_callback() {
  GLFWwindow* w_ptr = Window::instand().m_window;
  glfwSetCursorEnterCallback(w_ptr, cursor_enter_callback);
  glfwSetCursorPosCallback(w_ptr, cursor_pos_callback);
  glfwSetMouseButtonCallback(w_ptr, mouse_button_callback);
  glfwSetKeyCallback(w_ptr, key_callback);
  glfwSetWindowSizeCallback(w_ptr, window_size_callback);
}

void Event::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
  Input::set_cursor(xpos, ypos);
}

void Event::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
  if(button == GLFW_MOUSE_BUTTON_LEFT) {
    Input::set_mouse_down(Input::LEFT, action == GLFW_PRESS);
  } else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
    Input::set_mouse_down(Input::RIGHT, action == GLFW_PRESS);
  } else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
    Input::set_mouse_down(Input::MID, action == GLFW_PRESS);
  }
}

void Event::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  uint8_t _key = '0';
  
  switch(key) {
    case GLFW_KEY_W :
      _key = 'w';
      break;
    case GLFW_KEY_S :
      _key = 's';
      break;
    case GLFW_KEY_A :
      _key = 'a';
      break;
    case GLFW_KEY_D :
      _key = 'd';
      break;
  }

  Input::set_key_down(_key, action != GLFW_RELEASE);
}


void Event::window_size_callback(GLFWwindow* window, int width, int height) {
  
}

void Event::cursor_enter_callback(GLFWwindow* window, int entered) {
  if(entered) {
    std::cout<<"Cursor enters window"<<std::endl;
  }else {
    std::cout<<"Cursor leaves window"<<std::endl;
  }
}