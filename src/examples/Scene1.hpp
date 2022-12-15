#ifndef _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_
#define _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_

#include "core/Debug.h"
#include "core/Window.h"
#include "scene/Render.h"
#include "scene/Scene.h"

namespace scene {

using namespace component;
using namespace asset;
using namespace core;
class Scene1 : public Scene {
 public:
  Scene1(const std::string& title) : Scene(title) {}
  Entity quad;
  Entity main_camera;
  Entity cube;
  Entity sphere;

  std::shared_ptr<Shader> quad_shader;
  std::shared_ptr<Texture> quad_texture;

  virtual void init() override {
    add_fbo(Window::m_width, Window::m_height);
    CHECK_ERROR();

    quad_shader =
        std::make_shared<Shader>("../resource/shader/shader.vs", "../resource/shader/shader.fs");
    add_nor_ubo();
    set_nor_ubo(0U, quad_shader);
    quad_texture = std::make_shared<Texture>("../resource/texture/29720830.png", true);
    CHECK_ERROR();

    quad = create_entity("quad");
    quad.AddComponent<Mesh>(Mesh::primitive::QUAD);
    quad.AddComponent<Material>(quad_shader);
    quad.GetComponent<Material>().set_texture(0, quad_texture);
    quad.GetComponent<Transform>().translate(glm::vec3(-4, 0.0, 0.0));
    CHECK_ERROR();
    CORE_INFO("{} created", quad.name);

    cube = create_entity("cube");
    cube.AddComponent<Mesh>(Mesh::primitive::CUBE);
    cube.AddComponent<Material>(quad_shader);
    cube.GetComponent<Material>().set_texture(0, quad_texture);
    cube.GetComponent<Transform>().translate(glm::vec3(0.0, 0.0, 0.0));
    CHECK_ERROR();
    CORE_INFO("{} created", cube.name);

    sphere = create_entity("sphere");
    sphere.AddComponent<Mesh>(Mesh::primitive::SPHERE);
    sphere.AddComponent<Material>(quad_shader);
    sphere.GetComponent<Material>().set_texture(0, quad_texture);
    sphere.GetComponent<Transform>().translate(glm::vec3(4.0, 0.0, 0.0));
    CHECK_ERROR();
    CORE_INFO("{} created", sphere.name);

    main_camera = create_entity("main_camera");
    main_camera.AddComponent<CameraFps>(
        60.f, static_cast<float>(Window::m_width) / static_cast<float>(Window::m_height), 0.1f,
        100.f);
    // main_camera.AddComponent<Camera>(0.0f, 1.0f, 0.0f, 1.0f, 0.1f, 100.f);
    main_camera.GetComponent<Transform>().translate(glm::vec3(0.0, 0.0, 5.0));
    CHECK_ERROR();
    CORE_INFO("{} created", main_camera.name);

    Render::eable_depth_test();
    Render::eable_alpha_blend();
  }

  virtual void on_scene_render() override {
    auto& camera = main_camera.GetComponent<CameraFps>();
    camera.update();
    if (nor_ubo != nullptr) {
      glm::mat4 proj = main_camera.GetComponent<CameraFps>().get_projection_mat();
      glm::mat4 view = main_camera.GetComponent<CameraFps>().get_view_mat();
      nor_ubo->set_uniform(0, glm::value_ptr(proj));
      nor_ubo->set_uniform(1, glm::value_ptr(view));

      auto pos = main_camera.GetComponent<CameraFps>().T->get_position();
      auto forward = main_camera.GetComponent<CameraFps>().T->m_forward;
    }

    Render::clear_buffer();

    nor_fbo->bind();
    Render::clear_buffer();
    // 提交至渲染队列
    Render::Submit(quad.id);
    Render::Submit(sphere.id);
    Render::Submit(cube.id);

    Render::render_scene();

    nor_fbo->ubind();
    nor_fbo->draw();
  };
};

}  // namespace scene

#endif