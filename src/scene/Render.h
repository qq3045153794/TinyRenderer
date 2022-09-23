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
  static void render_scene(std::shared_ptr<asset::Shader> shader);
  static void draw_scene();

  template <typename... Args>
  static void Submit(Args&&... args) {
    (render_queue.push(args), ...);
  }

 private:
  static Scene* curr_scene;
  static std::queue<entt::entity> render_queue;
};

}  // namespace scene