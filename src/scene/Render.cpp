#include "scene/Render.h"

#include "core/Input.h"
#include "core/Window.h"
#include "scene/Scene.h"
#include "examples/Scene1.hpp"

namespace scene {

std::queue<entt::entity> Render::render_queue;
Scene* Render::curr_scene = nullptr;

Render::Render() {}

Render::~Render() {}

void Render::clear_buffer() {
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::Attach(const std::string& title) {
  Window::rename(title);
  Input::init();
  curr_scene = new Scene1(title);
  curr_scene->init();
}

void Render::render_scene(std::shared_ptr<asset::Shader> shader) {
  using namespace component;

  auto& reg = curr_scene->registry;
  auto mesh_group = reg.group<Mesh>(entt::get<Transform, Material>);

  while (!render_queue.empty()) {
    auto& e = render_queue.front();
    render_queue.pop();
    if (mesh_group.contains(e)) {
      auto& mesh = mesh_group.get<Mesh>(e);
      auto& transform = mesh_group.get<Transform>(e);
      auto& material = mesh_group.get<Material>(e);
      if (shader) {
        shader->bind();
        shader->set_uniform("model", transform.get_transform());
      } else {
        
        material.bind();
        material.set_uniform("model", transform.get_transform());
        std::cout << "get model" << std::endl;
        for (int j = 0; j < 4; j++) {
          for (int i = 0; i < 4; i ++) {
            std::cout << transform.get_transform()[i][j] << " ";
          }
          std::cout << std::endl;
        }
      }
      mesh.draw();
    }
  }
}

void Render::draw_scene() { curr_scene->on_scene_render(); }

void Render::eable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

void Render::eable_alpha_blend() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

}  // namespace scene