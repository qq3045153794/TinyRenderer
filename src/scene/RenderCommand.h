#ifndef _LRARN_OPENGL_SRC_SCENE_RENDER_H_
#define _LRARN_OPENGL_SRC_SCENE_RENDER_H_

#include <entt/entt.hpp>
#include <queue>

#include "asset/Shader.h"

namespace scene {

class Scene;
class RenderCommand {
 public:
  RenderCommand();
  ~RenderCommand();
  static Scene* get_scene();

  static void clear_buffer();
  static void flush();
  static void attach(const std::string& title);
  static void detach();
  static void reset();

  static void render_scene(std::shared_ptr<asset::Shader> shader = nullptr);
  static void draw_scene();
  static void draw_imGui();

  static void eable_depth_test(bool enable);
  static void eable_alpha_blend(bool enable);
  static void eable_face_culling(bool enable);
  static void eable_msaa(bool enalbe);

  static void set_front_is_ccw(bool is_ccw);
/*
  template <typename... Args>
  static void Submit(Args&&... args) {
    (render_queue.push(args), ...);
  }
*/
 private:
  static Scene* curr_scene;
  static Scene* last_scene;
  static std::queue<entt::entity> render_queue;
};

}  // namespace scene

#endif
