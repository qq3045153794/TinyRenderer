#ifndef _LRARN_OPENGL_SRC_SCENE_RENDER_H_
#define _LRARN_OPENGL_SRC_SCENE_RENDER_H_

#include <entt/entt.hpp>
#include <queue>

#include "asset/Shader.h"
#include "glad/glad.h"

namespace scene {

class Render {
 public:
  Render();
  ~Render();
  static void clear_buffer();
  static void Attach(const std::string& title);
  static void render_scene(std::shared_ptr<asset::Shader> shader = nullptr);
  static void draw_scene();

  static void eable_depth_test();
  static void eable_alpha_blend();

  template <typename... Args>
  static void Submit(Args&&... args) {
    (render_queue.push(args), ...);
  }

 private:
  static Scene* curr_scene;
  static std::queue<entt::entity> render_queue;
};

}  // namespace scene

#endif