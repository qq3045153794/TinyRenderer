#ifndef _LEARN_OPENGL_SRC_CORE_INPUT_H_
#define _LEARN_OPENGL_SRC_CORE_INPUT_H_

#include <unordered_map>

#include "glm/glm.hpp"

class Input {
 public:
  enum MouceButton { LEFT, RIGHT, MID };

  Input();
  ~Input();
  static void init();
  static void set_key_down(uint8_t key, bool pressed);
  static bool get_key_down(uint8_t key);
  static void set_mouse_down(MouceButton button, bool pressed);
  static bool get_mouse_down(MouceButton button);

  static void set_cursor(float new_x, float new_y);
  static glm::vec2 get_cursor();
  static float get_offset_x();
  static float get_offset_y();

 private:
  static std::unordered_map<uint8_t, bool> keybook;
  static inline float cursor_x = 0.f;
  static inline float cursor_y = 0.f;
  static inline float cursor_dx = 0.f;
  static inline float cursor_dy = 0.f;

  static inline bool mouse_button_l = false;
  static inline bool mouse_button_mid = false;
  static inline bool mouse_button_r = false;
};

#endif