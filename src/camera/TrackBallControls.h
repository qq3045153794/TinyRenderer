#ifndef _LEARN_OPENGL_SRC_CAMERA_TRACKBALL_CONTROLS_H_
#define _LEARN_OPENGL_SRC_CAMERA_TRACKBALL_CONTROLS_H_

#include "TrackBallInteractor.h"
namespace gl_cameras {

class TrackBallControls {
 public:
  static TrackBallControls& instance(){
    static TrackBallControls i;
    return i;
  }
  void init(GLFWwindow* window, int width, int height);
  void set_camera(Camera* c);
  void update();
 private:

  TrackBallControls() { };
  TrackBallControls(const TrackBallControls&) = delete;
  const TrackBallControls& operator=(const TrackBallControls&) = delete;
  
  static void mouse_move_call_back(GLFWwindow* window, double x, double y);
  static void mouse_button_call_back(GLFWwindow* window, int buttom, int action, int mods);
  static void size_call_back(GLFWwindow* window, int width, int height);
  Camera* camera_;
  GLFWwindow* window_;
  TrackBallInteractor interator_;

};

}
#endif