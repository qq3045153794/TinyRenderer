#include "scene/RenderCommand.h"

#include "component/Tag.hpp"
#include "core/Input.h"
#include "core/Log.h"
#include "core/Window.h"
#include "examples/Scene1.hpp"
#include "scene/Factory.hpp"
#include "scene/Scene.h"
#include "scene/ui.h"

namespace scene {

RenderCommand::RenderCommand() {}

RenderCommand::~RenderCommand() {}

void RenderCommand::clear_buffer() {
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::flush() {
  glfwSwapBuffers(core::Window::m_window);
  glfwPollEvents();
}


void RenderCommand::reset() {
  eable_alpha_blend(false);
  eable_depth_test(false);
  eable_face_culling(false);
}


void RenderCommand::eable_depth_test(bool enable) {
  if (enable) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void RenderCommand::eable_alpha_blend(bool enable) {
  if (enable) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glDisable(GL_BLEND);
  }
}

void RenderCommand::eable_face_culling(bool enable) {
  if (enable) {
    glEnable(GL_CULL_FACE);
    // 设置连接顺序 GL_CCW为逆时针 GL_CW为顺时针
    glFrontFace(GL_CCW);
    // 剔除背面 (剔除顺时针)
    glCullFace(GL_BACK);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

void RenderCommand::eable_msaa(bool enalbe) {
  static GLint buffers = 0, samples = 0, max_samples = 0;

  if (samples == 0) {
    glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
    glGetIntegerv(GL_SAMPLES, &samples);
    glGetIntegerv(GL_MAX_SAMPLES, &max_samples);
    CORE_TRACE("MSAA buffers {}", buffers);
    CORE_TRACE("MSAA samples {}", samples);
    CORE_TRACE("MSAA max_samples {}", max_samples);
    CORE_ASERT(buffers > 0,
               "MSAA buffers are not available! Check your window context...");
    // CORE_ASERT(samples == 4, "Invalid MSAA buffer size! 4 samples per pixel
    // is not available...");
  }

  if (enalbe) {
    glEnable(GL_MULTISAMPLE);
  } else {
    glDisable(GL_MULTISAMPLE);
  }
}

void RenderCommand::set_front_is_ccw(bool is_ccw) {
  glFrontFace(is_ccw ? GL_CCW : GL_CW);
}

}  // namespace scene
