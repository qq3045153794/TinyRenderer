/*
 * @Author       : xietao
 * @Date         : 2022-07-29 11:17:30
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-29 19:34:46
 * @FilePath     : /Learn_OpenGL/src/core/Event.h
 * @brief  :
 *
 * Copyright (c) 2022, All Rights Reserved.
 */
#ifndef _LEARN_OPENGL_SRC_CORE_H_
#define _LEARN_OPENGL_SRC_CORE_H_

#include "core/Input.h"
#include "core/Window.h"
namespace core {
class Event {
 public:
  Event();
  ~Event();
  /**
   * @brief : 将回调函数与窗口事件绑定
   * @return       {*}
   */
  static void register_callback();

 private:
  /**
   * @brief : 进入窗口回调
   * @param        {GLFWwindow*} window
   * @param        {int} entered
   * @return       {*}
   */
  static void cursor_enter_callback(GLFWwindow* window, int entered);

  /**
   * @brief        当鼠标移动时 调用回调 并将鼠标状态写入输入事件中
   * @param        {GLFWwindow*} window
   * @param        {double} xpos
   * @param        {double} ypos
   * @return       {*}
   */
  static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

  /**
   * @brief : 当鼠标按压时 调用回调 并将鼠标状态写入输入事件中
   * @param        {GLFWwindow*} window
   * @param        {int} button
   * @param        {int} action
   * @param        {int} mods
   * @return       {*}
   */
  static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

  /**
   * @brief : 当键盘按压时 调用回调 且将键盘状态写入输入事件中
   * @param        {GLFWwindow*} window 窗口指针
   * @param        {int} key 按键
   * @param        {int} scancode
   * @param        {int} action 按键状态
   * @param        {int} mods
   * @return       {*}
   */
  static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  /**
   * @brief :
   * @param        {GLFWwindow*} window
   * @param        {int} width
   * @param        {int} height
   * @return       {*}
   */
  static void window_size_callback(GLFWwindow* window, int width, int height);
};
}  // namespace core
#endif