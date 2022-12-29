/*
 * @Author       : xietao
 * @Date         : 2022-08-03 20:22:11
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-29 19:42:54
 * @FilePath     : /Learn_OpenGL/src/core/Input.h
 * @brief  : 插入系统 通过事件(Event)系统获取窗口事件
 *                 的数据，供全局调用
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _LEARN_OPENGL_SRC_CORE_INPUT_H_
#define _LEARN_OPENGL_SRC_CORE_INPUT_H_

#include <unordered_map>

#include "glm/glm.hpp"

namespace core {
class Input {
 public:
  enum MouceButton { LEFT, RIGHT, MID };

  Input();
  ~Input();
  static void init();
  /**
   * @brief : 设置键盘按键
   * @param        {uint8_t} key
   * @param        {bool} pressed
   * @return       {*}
   */
  static void set_key_down(uint8_t key, bool pressed);

  /**
   * @brief : 获取某个按键是否按压
   * @param        {uint8_t} key
   * @return       {*}
   */
  static bool get_key_down(uint8_t key);

  /**
   * @brief : 设置鼠标某个按键
   * @param        {MouceButton} button
   * @param        {bool} pressed
   * @return       {*}
   */
  static void set_mouse_down(MouceButton button, bool pressed);

  /**
   * @brief : 获取鼠标是否按压
   * @param        {MouceButton} button
   * @return       {*}
   */
  static bool get_mouse_down(MouceButton button);

  /**
   * @brief : 设置鼠标光标在像素坐标下位置
   * @param        {float} new_x
   * @param        {float} new_y
   * @return       {*}
   */
  static void set_cursor(float new_x, float new_y);

  /**
   * @brief : 设置是否是第一次进入窗口
   * @param        {bool} is_first_enter
   * @return       {*}
   */
  static void set_first_enter_window(bool is_first_enter);
  /**
   * @brief : 获取鼠标光标在像素坐标下位置
   * @return       {*}
   */
  static glm::vec2 get_cursor();

  /**
   * @brief : 鼠标在x轴偏离窗口中心
   * @return       {*}
   */
  static float get_offset_x();

  /**
   * @brief : 鼠标在y轴偏离窗口中心
   * @return       {*}
   */
  static float get_offset_y();

  static void clear();

 private:
  static std::unordered_map<uint8_t, bool> keybook;
  static inline float cursor_x = 0.f;
  static inline float cursor_y = 0.f;
  static inline float cursor_dx = 0.f;
  static inline float cursor_dy = 0.f;

  static inline bool mouse_button_l = false;
  static inline bool mouse_button_mid = false;
  static inline bool mouse_button_r = false;

  // 判断是否鼠标开始进入窗口 防止刚开始进入窗口鼠标距离窗口中央过大
  static inline bool first_enter_window = true;
};
}  // namespace core
#endif