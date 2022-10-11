#ifndef _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_
#define _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_

#include "core/Window.h"
#include "scene/Render.h"
#include "scene/Scene.h"

namespace scene {

using namespace component;
using namespace asset;
class Scene1 : public Scene {
 public:

  Scene1(const std::string& title)
    :Scene(title) {

  }
  Entity quad;
  Entity main_camera;

  std::shared_ptr<Shader> quad_shader;
  std::shared_ptr<Texture> quad_texture;

  virtual void init() override {
    add_nor_ubo();

    quad_shader =
        std::make_shared<Shader>("../resource/shader/shader.vs", "../resource/shader/shader.fs");
    set_nor_ubo(0U, quad_shader);

    quad_texture = std::make_shared<Texture>("../resource/texture/29720830.png");

    quad = create_entity("quad");
    quad.AddComponent<Mesh>(Mesh::primitive::QUAD);
    quad.AddComponent<Material>(quad_shader);
    quad.GetComponent<Material>().set_texture(0, quad_texture);

    main_camera = create_entity("main_camera");
    main_camera.AddComponent<Camera>(
        30, static_cast<float>(Window::m_width) / static_cast<float>(Window::m_height), 0.1, 100);
    main_camera.GetComponent<Transform>().translate(glm::vec3(0.0, 0.0, 1.0));

    Render::eable_depth_test();
    Render::eable_alpha_blend();
  }

  virtual void on_scene_render() override {
    if (nor_ubo != nullptr) {
      glm::mat4 proj = main_camera.GetComponent<Camera>().get_projection_mat();
      glm::mat4 view = main_camera.GetComponent<Camera>().get_view_mat();
      nor_ubo->set_uniform(0, glm::value_ptr(proj));
      nor_ubo->set_uniform(1, glm::value_ptr(view));
    }

    Render::Submit(quad.id);
    Render::render_scene();
    Render::clear_buffer();
  };
};

}  // namespace scene

#endif