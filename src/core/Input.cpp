#include "core/Input.h"

Input::Input() {}

Input::~Input() {}

std::unordered_map<uint8_t, bool> Input::keybook = {
    {'w', false}, {'s', false}, {'a', false}, {'d', false}
};

void Input::set_key_down(uint8_t key, bool pressed) {
  if(keybook.find(key) == keybook.end()){
    return;
  }
  keybook[key] = pressed;
}

bool Input::get_key_down(uint8_t key) {
  if(keybook.find(key) == keybook.end()){
    return false;
  }
  return keybook[key];
}

void Input::set_mouse_down(MouceButton button, bool pressed) {
  if(button == LEFT) {
    mouse_button_l = pressed;
  } else if(button == MID) {
    mouse_button_mid = pressed;
  } else if(button == RIGHT) {
    mouse_button_r = pressed;
  }
}

bool Input::get_mouse_down(MouceButton button) {
  if(button == LEFT) {
    return mouse_button_l; 
  } else if(button == MID) {
    return mouse_button_mid;
  } else if(button == RIGHT) {
    return mouse_button_r;
  }
  return false;
}

void Input::set_cursor(float new_x, float new_y) {
  cursor_dx = new_x - cursor_x;
  cursor_dy = new_y - cursor_y;
  cursor_x = new_x;
  cursor_y = new_y;

}

glm::vec2 Input::get_cursor() {
  return glm::vec2(cursor_x, cursor_y);
}

float Input::get_offset_x() {
  return cursor_dx;
}

float Input::get_offset_y() {
  cursor_dy
}



