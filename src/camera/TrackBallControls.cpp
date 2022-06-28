#include "TrackBallControls.h"

namespace gl_cameras {

void TrackBallControls::init(GLFWwindow* window, int width, int height) {
  window_ = window;
  glfwSetCursorPosCallback(window_, mouse_move_call_back);
  glfwSetMouseButtonCallback(window_, mouse_button_call_back);
  glfwSetWindowSizeCallback(window_, size_call_back);
  size_call_back(window, width, height);
}

void TrackBallControls::set_camera(Camera* c) {
  camera_ = c;
  // TODO
  interator_.set_camera(c);
}

void TrackBallControls::mouse_move_call_back(GLFWwindow* window, double x, double y) {
  instance().interator_.set_click_point(x, y);
}

void TrackBallControls::mouse_button_call_back(GLFWwindow* window, 
                                               int button, 
                                               int action, 
                                               int mods) {
  switch(action) {
    case GLFW_PRESS: {
      switch(button) {
        case  GLFW_MOUSE_BUTTON_LEFT:
          instance().interator_.set_is_left_click(true);
          break;
      }
      double x_pos, y_pos;
      glfwGetCursorPos(window, &x_pos, &y_pos);
      instance().interator_.set_click_point(x_pos, y_pos);
      break;
    }
    case GLFW_RELEASE: {
      switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
          instance().interator_.set_is_left_click(false);
      }
      break;
    }
    default :
      break;
  }
}

void TrackBallControls::size_call_back(GLFWwindow* window, int width, int height) {
  instance().interator_.set_screen_size(static_cast<GLfloat>(width), static_cast<GLfloat>(height));
}

}