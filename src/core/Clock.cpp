#include "core/Clock.h"
#include "GLFW/glfw3.h"

Clock::Clock() {}

Clock::~Clock() {}

void Clock::update() {

  last_frame = this_frame;  
  this_frame = static_cast<float>(glfwGetTime());

  dt = this_frame - last_frame;
}