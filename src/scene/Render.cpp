#include "scene/Render.h"

#include "component/Tag.hpp"
#include "core/Input.h"
#include "core/Log.h"
#include "core/Window.h"
#include "examples/Scene1.hpp"
#include "scene/Factory.hpp"
#include "scene/Scene.h"
#include "scene/ui.h"

namespace scene {

std::queue<entt::entity> Render::render_queue;
Scene* Render::curr_scene = nullptr;
Scene* Render::last_scene = nullptr;

Render::Render() {}

Render::~Render() {}

Scene* Render::get_scene() { return curr_scene; }

void Render::clear_buffer() {
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::flush() {
  glfwSwapBuffers(Window::m_window);
  glfwPollEvents();
}

void Render::attach(const std::string& title) {
  Window::rename(title);
  Input::clear();

  Window::layer = Layer::ImGui;

  Scene* new_scene = factory::LoadScene(title);
  std::swap(curr_scene, new_scene);
  // curr_scene = new_scene;
  curr_scene->init();
}

void Render::detach() {
  CORE_TRACE("Detaching scene \"{0}\" ......", curr_scene->m_title);
  last_scene = curr_scene;
  curr_scene = nullptr;

  delete last_scene;
  last_scene = nullptr;

  Render::reset();
}

void Render::reset() {
  eable_alpha_blend(false);
  eable_depth_test(false);
  eable_face_culling(false);
}

void Render::render_scene(std::shared_ptr<asset::Shader> shader) {
  using namespace component;

  auto& reg = curr_scene->registry;
  auto mesh_group = reg.group<Mesh>(entt::get<Transform, Material, Tag>);
  auto model_group = reg.group<Model>(entt::get<Transform, Tag>);

  while (!render_queue.empty()) {
    auto& e = render_queue.front();
    render_queue.pop();
    if (mesh_group.contains(e)) {
      auto& mesh = mesh_group.get<Mesh>(e);
      auto& transform = mesh_group.get<Transform>(e);
      auto& material = mesh_group.get<Material>(e);
      auto& tag = mesh_group.get<Tag>(e);
      if (shader) {
        shader->bind();
        shader->set_uniform("model", transform.get_transform());
      } else {
        material.bind();
        material.set_uniform("model", transform.get_transform());
      }
      if (tag.contains(ETag::Skybox)) {
        set_front_is_ccw(false);
        mesh.draw();
        set_front_is_ccw(true);
      } else {
        mesh.draw();
      }
    } else if (model_group.contains(e)) {
      auto& model = model_group.get<Model>(e);
      auto& transform = model_group.get<Transform>(e);
      for (auto& mesh : model.meshes) {
        auto& material = model.materials.at(mesh.material_id);
        if (shader) {
          // TODO
        } else {
          material.bind();
          material.set_uniform("model", transform.get_transform());
        }
        mesh.draw();
      }
    }
  }
}

void Render::draw_scene() { curr_scene->on_scene_render(); }

void Render::draw_imGui() {
  bool switch_scene = false;
  std::string next_scene_title;
  if (ui::new_frame(); true) {
    // 渲染菜单栏
    ui::draw_menu_bar(next_scene_title);

    if (!next_scene_title.empty()) {
      switch_scene = true;
      Render::clear_buffer();
      ui::draw_loading_screen();

    } else {
      if (Window::layer == Layer::ImGui) {
        curr_scene->on_imgui_render();
      }
    }
    ui::end_frame();
  }
  // 双缓存 将渲染画面提交到窗口
  Render::flush();

  if (switch_scene) {
    detach();
    attach(next_scene_title);
  }
}

void Render::eable_depth_test(bool enable) {
  if (enable) {
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void Render::eable_alpha_blend(bool enable) {
  if (enable) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glDisable(GL_BLEND);
  }
}

void Render::eable_face_culling(bool enable) {
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

void Render::set_front_is_ccw(bool is_ccw) { glFrontFace(is_ccw ? GL_CCW : GL_CW); }

}  // namespace scene