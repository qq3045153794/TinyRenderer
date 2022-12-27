#include "core/Input.h"

#include <iostream>

#include "core/Window.h"

namespace core {
// 键盘案件映射表
std::unordered_map<uint8_t, bool> Input::keybook = {
    {'w', false}, {'s', false}, {'a', false}, {'d', false}, {'q', false}, {'e', false}, {'\r', false}};

Input::Input() {}

Input::~Input() {}

void Input::init() {
  clear();
}

void Input::clear() {
  for (auto& keystate : keybook) {
    keystate.second = 0;
  }

  cursor_x = Window::m_width * 0.5f;
  cursor_y = Window::m_height * 0.5f;
  set_cursor(cursor_x, cursor_y);

  cursor_dx = 0.f;
  cursor_dy = 0.f;

  mouse_button_l = false;
  mouse_button_mid = false;
  mouse_button_r = false;
  first_enter_window = true;
}

void Input::set_key_down(uint8_t key, bool pressed) {
  if (keybook.find(key) == keybook.end()) {
    return;
  }
  keybook[key] = pressed;
}

bool Input::get_key_down(uint8_t key) {
  if (keybook.find(key) == keybook.end()) {
    return false;
  }
  return keybook[key];
}

void Input::set_mouse_down(MouceButton button, bool pressed) {
  if (button == LEFT) {
    mouse_button_l = pressed;
  } else if (button == MID) {
    mouse_button_mid = pressed;
  } else if (button == RIGHT) {
    mouse_button_r = pressed;
  }
}

bool Input::get_mouse_down(MouceButton button) {
  if (button == LEFT) {
    return mouse_button_l;
  } else if (button == MID) {
    return mouse_button_mid;
  } else if (button == RIGHT) {
    return mouse_button_r;
  }
  return false;
}

void Input::set_cursor(float new_x, float new_y) {
  if (first_enter_window) {
    cursor_dx = 0.0;
    cursor_dy = 0.0;
    first_enter_window = false;
  } else {
    cursor_dx = new_x - cursor_x;
    cursor_dy = new_y - cursor_y;
  }
  // 让鼠标一直居中
  glfwSetCursorPos(Window::m_window, cursor_x, cursor_y);
}

void Input::set_first_enter_window(bool is_first_enter) {
  first_enter_window = is_first_enter;
}

glm::vec2 Input::get_cursor() { return glm::vec2(cursor_x, cursor_y); }

float Input::get_offset_x() {
  int tmp = -cursor_dx;
  cursor_dx = 0;
  return tmp;
}

float Input::get_offset_y() {
  int tmp = -cursor_dy;
  cursor_dy = 0;
  return tmp;
}
}