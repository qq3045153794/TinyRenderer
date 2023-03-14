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

  static void clear_buffer();
  static void flush();
  static void reset();
  static void render_scene(std::shared_ptr<asset::Shader> shader = nullptr);
  static void eable_depth_test(bool enable);
  static void eable_alpha_blend(bool enable);
  static void eable_face_culling(bool enable);
  static void eable_msaa(bool enalbe);

  static void set_front_is_ccw(bool is_ccw);
};

}  // namespace scene

#endif
