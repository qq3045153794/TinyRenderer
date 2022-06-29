#ifndef _AVM_SRC_CAMERA_TRACKBALL_CONTROLS_H_
#define _AVM_SRC_CAMERA_TRACKBALL_CONTROLS_H_

#include <GLFW/glfw3.h>

#include <iostream>

#include "TrackBallInteractor.h"

namespace gl_camera {

// 相机轨迹球控制器
// 单例使用 gl_camera::TrackballControls::instance()
// 监听window事件，调用
class TrackballControls {
 public:
  TrackballControls();
  ~TrackballControls();

  static TrackballControls &instance();
  // 窗口回调
  static void buttonCallback(GLFWwindow *window, int button, int action, int mods);
  static void errorCallback(int error, const char *description);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
  static void moveCallback(GLFWwindow *window, double xpos, double ypos);
  static void scrollCallback(GLFWwindow *window, double xpos, double ypos);
  static void sizeCallback(GLFWwindow *window, int width, int height);

  void init(GLFWwindow *window, int width, int height);
  void setCamera(Camera *c);
  void update();

  Camera *mCamera;
  GLFWwindow *mWindow;
  TrackBallInteractor mInteractor;
};

}  // namespace gl_camera

#endif
