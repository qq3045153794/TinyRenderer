#include <queue>

#include "glad/glad.h"
#include <entt/entt.hpp>

namespace scene {

class Render {

 public:
  Render();
  ~Render();
  static void clear_buffer();

  static void render_scene();

  template<typename... Args>
  static void Submit(Args&&... args) {
    (render_queue.push(args), ...);
   }
 private:
  static std::queue<entt::entity> render_queue;
};

}  // namespace scene