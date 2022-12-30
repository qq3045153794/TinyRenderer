#ifndef _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_
#define _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_

#include "core/Debug.h"
#include "core/Window.h"
#include "scene/Render.h"
#include "scene/Scene.h"
#include "scene/ui.h"
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
  Entity skybox;
  Entity paimon;

  std::shared_ptr<Shader> quad_shader;
  std::shared_ptr<Shader> skybox_shader;
  std::shared_ptr<Shader> paimon_shader;

  std::shared_ptr<Texture> skybox_texture;
  std::shared_ptr<Texture> quad_texture;
  
  std::shared_ptr<Material> paimon_1;
  
  virtual void init() override {
    add_fbo(Window::m_width, Window::m_height);

    quad_shader =
        std::make_shared<Shader>("../resource/shader/shader.vs", "../resource/shader/shader.fs");

    skybox_shader = std::make_shared<Shader>("../resource/shader/skybox.vs", "../resource/shader/skybox.fs");

    

    add_nor_ubo();
    set_nor_ubo(0U, quad_shader);

    quad_texture = std::make_shared<Texture>("../resource/texture/29720830.png", true);

    skybox_texture = std::make_shared<Texture>(std::vector<std::string>{
        "../resource/texture/skybox/right.jpg", "../resource/texture/skybox/left.jpg",
        "../resource/texture/skybox/top.jpg", "../resource/texture/skybox/bottom.jpg",
        "../resource/texture/skybox/front.jpg", "../resource/texture/skybox/back.jpg"});

    paimon_1 = std::make_shared<Material>(quad_shader);
    

    CHECK_ERROR();

    skybox = create_entity("skybox", ETag::Skybox);
    skybox.AddComponent<Mesh>(Mesh::primitive::CUBE);
    skybox.AddComponent<Material>(skybox_shader);
    skybox.GetComponent<Material>().set_texture(0, skybox_texture);
    // skybox.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 0.0));

    quad = create_entity("quad");
    quad.AddComponent<Mesh>(Mesh::primitive::QUAD);
    quad.AddComponent<Material>(quad_shader);
    quad.GetComponent<Material>().set_texture(0, quad_texture);
    quad.GetComponent<Transform>().set_position(glm::vec3(-4, 0.0, 0.0));
    CHECK_ERROR();
    CORE_INFO("{} created", quad.name);

    cube = create_entity("cube");
    cube.AddComponent<Mesh>(Mesh::primitive::CUBE);
    cube.AddComponent<Material>(quad_shader);
    cube.GetComponent<Material>().set_texture(0, quad_texture);
    cube.GetComponent<Transform>().translate(glm::vec3(8.0, 0.0, 0.0));
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
    main_camera.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 5.0));
    CHECK_ERROR();
    
    paimon = create_entity("paimon");
    paimon.AddComponent<Model>("../resource/objects/paimon/untitled.obj", Quality::Auto);
    paimon.GetComponent<Transform>().set_position(glm::vec3(0.0, -2.0, 0.0));
    paimon.GetComponent<Transform>().set_scale(glm::vec3(4.0, 4.0, 4.0));

    auto& temp_mat_1 = paimon.GetComponent<Model>().SetMatermial("披风", *paimon_1);
    temp_mat_1.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/披风2.jpg"));

    auto& temp_mat_2 = paimon.GetComponent<Model>().SetMatermial("头发", *paimon_1);
    temp_mat_2.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/头发.jpg"));

    auto& temp_mat_3 = paimon.GetComponent<Model>().SetMatermial("衣服", *paimon_1);
    temp_mat_3.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/衣服.jpg"));

    auto& temp_mat_4 = paimon.GetComponent<Model>().SetMatermial("皮肤", *paimon_1);
    temp_mat_4.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/衣服.jpg"));

    auto& temp_mat_5 = paimon.GetComponent<Model>().SetMatermial("眼睛", *paimon_1);
    temp_mat_5.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/头发.jpg"));

    auto& temp_mat_6 = paimon.GetComponent<Model>().SetMatermial("表情", *paimon_1);
    temp_mat_6.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/表情.png"));

    auto& temp_mat_7 = paimon.GetComponent<Model>().SetMatermial("脸", *paimon_1);
    temp_mat_7.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/脸.jpg"));

    auto& temp_mat_8 = paimon.GetComponent<Model>().SetMatermial("眼白", *paimon_1);
    temp_mat_8.set_texture(0, std::make_shared<Texture>("../resource/objects/paimon/Texture/脸.jpg"));

    CHECK_ERROR();

    Render::eable_depth_test();
    Render::eable_alpha_blend();
    Render::eable_face_culling();
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
      auto up = main_camera.GetComponent<CameraFps>().T->m_up;

    }

    Render::clear_buffer();

    nor_fbo->bind();
    Render::clear_buffer();
    // 提交至渲染队列
    
    Render::Submit(quad.id);
    Render::Submit(sphere.id);
    Render::Submit(cube.id);
    Render::Submit(paimon.id);

    Render::Submit(skybox.id);

    Render::render_scene();

    nor_fbo->ubind();
    nor_fbo->draw();
  };

  virtual void on_imgui_render() override {
    ui::draw_Gizmo(main_camera, cube, ui::Gizmo::TRANSLATE);
  }
};

}  // namespace scene

#endif