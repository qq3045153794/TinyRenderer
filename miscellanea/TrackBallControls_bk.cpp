#include "TrackBallControls.h"

namespace gl_camera {

TrackballControls::TrackballControls(){};
TrackballControls::~TrackballControls(){};

TrackballControls &TrackballControls::instance() {
  static TrackballControls i;
  return i;
}

void TrackballControls::buttonCallback(GLFWwindow *window, int button, int action, int mods) {
  switch (action) {
    case GLFW_PRESS: {
      switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
          instance().mInteractor.setLeftClicked(true);
          break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
          instance().mInteractor.setMiddleClicked(true);
          break;
        case GLFW_MOUSE_BUTTON_RIGHT:
          instance().mInteractor.setRightClicked(true);
          break;
      }

      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      instance().mInteractor.setClickPoint(xpos, ypos);
      break;
    }
    case GLFW_RELEASE: {
      switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
          instance().mInteractor.setLeftClicked(false);
          break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
          instance().mInteractor.setMiddleClicked(false);
          break;
        case GLFW_MOUSE_BUTTON_RIGHT:
          instance().mInteractor.setRightClicked(false);
          break;
      }
      break;
    }
    default:
      break;
  }
}

void TrackballControls::keyCallback(GLFWwindow *window, int key, int scancode, int action,
                                    int mods) {
  float length;

  switch (action) {
    case GLFW_PRESS:
      switch (key) {
        case GLFW_KEY_ESCAPE:
          // Exit app on ESC key.
          glfwSetWindowShouldClose(window, GL_TRUE);
          break;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
          instance().mInteractor.setSpeed(5.f);
          break;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
          instance().mInteractor.setSpeed(.1f);
          break;
        case GLFW_KEY_F1:
          // instance().mAnimator.setAnimation(Animator::ORBIT);
          break;
        case GLFW_KEY_C:
          std::cout << "Eye "
                    << "(" << instance().mCamera->getEye().x << ","
                    << instance().mCamera->getEye().y << "," << instance().mCamera->getEye().z
                    << ") "
                    << "Center "
                    << "(" << instance().mCamera->getCenter().x << ","
                    << instance().mCamera->getCenter().y << "," << instance().mCamera->getCenter().z
                    << ") "
                    << "Up "
                    << "(" << instance().mCamera->getUp().x << "," << instance().mCamera->getUp().y
                    << "," << instance().mCamera->getUp().z << ")\n";
          break;
        case GLFW_KEY_R:
          // Reset the view.
          instance().mCamera->reset();
          instance().mInteractor.setCamera(instance().mCamera);
          break;
        case GLFW_KEY_T:
          // Toogle motion type.
          if (instance().mInteractor.getMotionRightClick() == TrackBallInteractor::FIRSTPERSON) {
            instance().mInteractor.setMotionRightClick(TrackBallInteractor::PAN);
          } else {
            instance().mInteractor.setMotionRightClick(TrackBallInteractor::FIRSTPERSON);
          }
          break;
        case GLFW_KEY_X:
          // Snap view to axis.
          length = glm::length(instance().mCamera->getEye() - instance().mCamera->getCenter());
          instance().mCamera->setEye(length, 0, 0);
          instance().mCamera->setUp(0, 1, 0);
          instance().mCamera->update();
          instance().mInteractor.setCamera(instance().mCamera);
          break;
        case GLFW_KEY_Y:
          length = glm::length(instance().mCamera->getEye() - instance().mCamera->getCenter());
          instance().mCamera->setEye(0, length, 0);
          instance().mCamera->setUp(1, 0, 0);
          instance().mCamera->update();
          instance().mInteractor.setCamera(instance().mCamera);
          break;
        case GLFW_KEY_Z:
          length = glm::length(instance().mCamera->getEye() - instance().mCamera->getCenter());
          instance().mCamera->setEye(0, 0, length);
          instance().mCamera->setUp(1, 0, 0);
          instance().mCamera->update();
          instance().mInteractor.setCamera(instance().mCamera);
          break;
        default:
          break;
      }
      break;
    case GLFW_RELEASE:
      switch (key) {
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
          instance().mInteractor.setSpeed(1.f);
          break;
      }
      break;
    default:
      break;
  }
}

void TrackballControls::moveCallback(GLFWwindow *window, double xpos, double ypos) {
  instance().mInteractor.setClickPoint(xpos, ypos);
}

void TrackballControls::scrollCallback(GLFWwindow *window, double xpos, double ypos) {
  instance().mInteractor.setScrollDirection(xpos + ypos > 0 ? true : false);
}

void TrackballControls::sizeCallback(GLFWwindow *window, int width, int height) {
  instance().mInteractor.setScreenSize(width, height);
}

void TrackballControls::init(GLFWwindow *window, int width, int height) {
  mWindow = window;
  glfwSetCursorPosCallback(mWindow, &TrackballControls::moveCallback);
  glfwSetKeyCallback(mWindow, &TrackballControls::keyCallback);
  glfwSetMouseButtonCallback(mWindow, &TrackballControls::buttonCallback);
  glfwSetScrollCallback(mWindow, &TrackballControls::scrollCallback);
  glfwSetWindowSizeCallback(mWindow, &TrackballControls::sizeCallback);
  sizeCallback(window, width, height);
};

void TrackballControls::setCamera(Camera *c) {
  mCamera = c;
  // for reset value
  mCamera->setDefault(mCamera->getEye(), mCamera->getCenter(), mCamera->getUp());
  mInteractor.setCamera(c);
};

void TrackballControls::update() { mInteractor.update(); };

};  // namespace gl_camera
