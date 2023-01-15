#ifndef _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_
#define _LRARN_OPENGL_SRC_EAXMPLES_SCENE1_H_
#include "component/Light.h"
#include "core/Debug.h"
#include "core/Window.h"
#include "scene/Render.h"
#include "scene/Scene.h"
#include "scene/ui.h"
namespace scene {

using namespace component;
using namespace asset;
using namespace core;

static glm::vec4 sphere_albedo{0.22f, 0.0f, 1.0f, 1.0f};
static float sphere_metalness = 0.05f;
static float sphere_roughness = 0.2f;
static float sphere_ao = 1.0f;
static float plane_roughness = 0.1f;

class Scene1 : public Scene {
 public:
  Scene1(const std::string& title) : Scene(title) {}
  Entity quad;
  Entity main_camera;
  Entity cube;
  Entity sphere;
  Entity skybox;
  Entity skybox_hdr;
  Entity paimon;
  Entity sun_light;
  Entity point_light;
  Entity camera_light; 

  std::shared_ptr<Shader> quad_shader;
  std::shared_ptr<Shader> skybox_shader;
  std::shared_ptr<Shader> ptr_shader;
  std::shared_ptr<Shader> skybox_hdr_shader;

  std::shared_ptr<Texture> skybox_texture;
  std::shared_ptr<Texture> skybox_hdr_texutre;
  std::shared_ptr<Texture> quad_texture;

  std::shared_ptr<Material> paimon_1;

  void add_ubo(std::shared_ptr<asset::Shader> shader) override {
    // camera
    {
      std::vector<GLuint> offset = {0U, 64U, 128U, 144U};
      std::vector<GLuint> lenght = {64U, 64U, 16U, 16U};
      GLuint sz = 160U;
      UBOs.try_emplace(0U, offset, lenght, sz);
      UBOs[0].set_binding(0U, "Matrices", shader->get_id());
    }

    // 平行光照
    {
      std::vector<GLuint> offset = {0U, 16U, 32U};
      std::vector<GLuint> lenght = {16U, 16U, 4U};
      GLuint sz = 36U;
      UBOs.try_emplace(1U, offset, lenght, sz);
      UBOs[1].set_binding(1U, "DL", shader->get_id());
    }

    // 点光源
    {
      std::vector<GLuint> offset = {0U, 16U, 32U, 36U, 40U, 44U};
      std::vector<GLuint> lenght = {16U, 16U, 4U, 4U, 4U, 4U};
      GLuint sz = 48U;
      UBOs.try_emplace(2U, offset, lenght, sz);
      UBOs[2].set_binding(2U, "PL", shader->get_id());
    }

    // 相机光
    {
      std::vector<GLuint> offset = {0U, 16U, 32U, 48U, 52U, 56U, 60U};
      std::vector<GLuint> lenght = {16U, 16U, 16U, 4U, 4U, 4U, 4U};
      GLuint sz = 64U;
      UBOs.try_emplace(3U, offset, lenght, sz);
      UBOs[3].set_binding(3U, "SL", shader->get_id());
    }
  }

  virtual void init() override {
    add_fbo(Window::m_width, Window::m_height);

    quad_shader =
        std::make_shared<Shader>("../resource/shader/shader.vs", "../resource/shader/shader.fs");

    skybox_shader =
        std::make_shared<Shader>("../resource/shader/skybox.vs", "../resource/shader/skybox.fs");

    ptr_shader = std::make_shared<Shader>("../resource/shader/pbr.vs", "../resource/shader/pbr.fs");

    skybox_hdr_shader = std::make_shared<Shader>("../resource/shader/skybox_hdr.vs", "../resource/shader/skybox_hdr.fs");

    add_ubo(quad_shader);
    add_ubo(skybox_shader);
    add_ubo(ptr_shader);

    quad_texture = std::make_shared<Texture>("../resource/texture/29720830.png", true);

    skybox_texture = std::make_shared<Texture>(std::vector<std::string>{
        "../resource/texture/skybox/right.jpg", "../resource/texture/skybox/left.jpg",
        "../resource/texture/skybox/top.jpg", "../resource/texture/skybox/bottom.jpg",
        "../resource/texture/skybox/front.jpg", "../resource/texture/skybox/back.jpg"});

    skybox_hdr_texutre = std::make_shared<Texture>("../resource/texture/Milkyway_small.hdr", 512);

    paimon_1 = std::make_shared<Material>(quad_shader);

    CHECK_ERROR();

    sun_light = create_entity("sum light");
    sun_light.AddComponent<DirectionLight>(glm::vec3(1.0, 1.0, 1.0), 1.0);
    sun_light.GetComponent<Transform>().set_ealar_YZX(glm::vec3(-45, 0.0, 0.0));
    if (auto& ubo = UBOs[1]; true) {
      auto& dl = sun_light.GetComponent<DirectionLight>();
      auto& dt = sun_light.GetComponent<Transform>();
      auto color = glm::vec4(dl.m_color, 1.0);
      auto directionl = glm::vec4(-dt.m_forward, 0.0);
      auto intensity = dl.m_intensity;
      ubo.set_uniform(0, glm::value_ptr(color));
      ubo.set_uniform(1, glm::value_ptr(directionl));
      ubo.set_uniform(2, &intensity);
    }

    point_light = create_entity("point light");
    point_light.AddComponent<PointLight>(glm::vec3(1.0, 1.0, 1.0), 1.0);
    point_light.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, -5.0));
    point_light.GetComponent<PointLight>().set_attenuation(0.09f, 0.032f);
    if (auto& ubo = UBOs[2]; true) {
      auto& dl = point_light.GetComponent<PointLight>();
      auto& dt = point_light.GetComponent<Transform>();
      auto color = glm::vec4(dl.m_color, 1.0);
      auto position = glm::vec4(dt.get_position(), 1.0);
      auto intensity = dl.m_intensity;
      auto linear = dl.m_linear;
      auto quadratic = dl.m_quadratic;
      auto range = dl.m_range;

      ubo.set_uniform(0, glm::value_ptr(color));
      ubo.set_uniform(1, glm::value_ptr(position));
      ubo.set_uniform(2, &intensity);
      ubo.set_uniform(3, &linear);
      ubo.set_uniform(4, &quadratic);
      ubo.set_uniform(5, &range);

    }
    camera_light = create_entity("camera light");
    camera_light.AddComponent<Spotlight>(glm::vec3(1.0, 0.553, 0.0), 3.8);
    camera_light.GetComponent<Spotlight>().set_cutoff(12.0f);

    skybox = create_entity("skybox", ETag::Skybox);
    skybox.AddComponent<Mesh>(Mesh::primitive::CUBE);
    skybox.AddComponent<Material>(skybox_shader);
    skybox.GetComponent<Material>().set_texture(0, skybox_texture);
    // skybox.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 0.0));

    skybox_hdr = create_entity("skybox hdr", ETag::Skybox);
    skybox_hdr.AddComponent<Mesh>(Mesh::primitive::CUBE);
    skybox_hdr.AddComponent<Material>(skybox_hdr_shader);
    skybox_hdr.GetComponent<Material>().set_texture(0, skybox_hdr_texutre);

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
    sphere.GetComponent<Transform>().translate(glm::vec3(4.0, 0.0, 0.0));
    sphere.AddComponent<Material>(ptr_shader);
    auto& sphere_mat = sphere.GetComponent<Material>();
    sphere_mat.bing_uniform(Material::pbr_u::albedo, sphere_albedo);
    sphere_mat.bing_uniform(Material::pbr_u::metalness, sphere_metalness);
    sphere_mat.bing_uniform(Material::pbr_u::roughness, sphere_roughness);
    sphere_mat.bing_uniform(Material::pbr_u::ao, sphere_ao);

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
    temp_mat_1.set_texture(
        0, std::make_shared<Texture>("../resource/objects/paimon/Texture/披风2.jpg"));

    auto& temp_mat_2 = paimon.GetComponent<Model>().SetMatermial("头发", *paimon_1);
    temp_mat_2.set_texture(
        0, std::make_shared<Texture>("../resource/objects/paimon/Texture/头发.jpg"));

    auto& temp_mat_3 = paimon.GetComponent<Model>().SetMatermial("衣服", *paimon_1);
    temp_mat_3.set_texture(
        0, std::make_shared<Texture>("../resource/objects/paimon/Texture/衣服.jpg"));

    auto& temp_mat_4 = paimon.GetComponent<Model>().SetMatermial("皮肤", *paimon_1);
    temp_mat_4.set_texture(
        0, std::make_shared<Texture>("../resource/objects/paimon/Texture/衣服.jpg"));

    auto& temp_mat_5 = paimon.GetComponent<Model>().SetMatermial("眼睛", *paimon_1);
    temp_mat_5.set_texture(
        0, std::make_shared<Texture>("../resource/objects/paimon/Texture/头发.jpg"));

    auto& temp_mat_6 = paimon.GetComponent<Model>().SetMatermial("表情", *paimon_1);
    temp_mat_6.set_texture(
        0, std::make_shared<Texture>("../resource/objects/paimon/Texture/表情.png"));

    auto& temp_mat_7 = paimon.GetComponent<Model>().SetMatermial("脸", *paimon_1);
    temp_mat_7.set_texture(0,
                           std::make_shared<Texture>("../resource/objects/paimon/Texture/脸.jpg"));

    auto& temp_mat_8 = paimon.GetComponent<Model>().SetMatermial("眼白", *paimon_1);
    temp_mat_8.set_texture(0,
                           std::make_shared<Texture>("../resource/objects/paimon/Texture/脸.jpg"));

    CHECK_ERROR();

    Render::eable_depth_test(true);
    Render::eable_alpha_blend(true);
    Render::eable_face_culling(true);
    // WSL不支持MSAA 暂时注释
    // Render::eable_msaa(true);
  }

  virtual void on_scene_render() override {
    auto& camera = main_camera.GetComponent<CameraFps>();
    camera.update();
    if (asset::UBO& ubo = UBOs[0]; true) {
      glm::mat4 proj = main_camera.GetComponent<CameraFps>().get_projection_mat();
      glm::mat4 view = main_camera.GetComponent<CameraFps>().get_view_mat();
      glm::vec4 pos = glm::vec4(main_camera.GetComponent<CameraFps>().T->get_position(), 1.0);
      glm::vec4 forward = glm::vec4(main_camera.GetComponent<CameraFps>().T->m_forward, 0.0);
      ubo.set_uniform(0, glm::value_ptr(proj));
      ubo.set_uniform(1, glm::value_ptr(view));
      ubo.set_uniform(2, glm::value_ptr(pos));
      ubo.set_uniform(3, glm::value_ptr(forward));
    }

    if(auto& ubo = UBOs[3]; true) {
      auto& camera = main_camera.GetComponent<Transform>();
      auto& sp = camera_light.GetComponent<Spotlight>();
      auto color =glm::vec4(sp.m_color, 1.0);
      auto pos = glm::vec4(camera.get_position(), 1.0);
      auto direction = glm::vec4(-camera.m_forward, 0.0);
      float intensity = sp.m_intensity;
      float inner_cos = sp.get_inner_cosine();
      float outer_cos = sp.get_outer_cosine();
      float range = sp.m_range;
      ubo.set_uniform(0, glm::value_ptr(color));
      ubo.set_uniform(1, glm::value_ptr(pos));
      ubo.set_uniform(2, glm::value_ptr(direction));
      ubo.set_uniform(3, &intensity);
      ubo.set_uniform(4, &inner_cos);
      ubo.set_uniform(5, &outer_cos);
      ubo.set_uniform(6, &range);
    }

    Render::clear_buffer();

    nor_fbo->bind();
    Render::clear_buffer();
    // 提交至渲染队列

    Render::Submit(quad.id);
    Render::Submit(sphere.id);
    Render::Submit(cube.id);
    Render::Submit(paimon.id);

    // Render::Submit(skybox.id);
    Render::Submit(skybox_hdr.id);
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
