#include <asset/Buffer.h>
#include <asset/FBO.h>
#include <component/Light.h>
#include <library/ShaderLibrary.h>
#include <library/TextureLibrary.h>
#include <scene/RenderCommand.h>
#include <scene/Scene.h>
#include <system/EnvironmentSystem.h>
namespace saber {
namespace system {

using Texture = asset::Texture;
using FBO = ::asset::FBO;
using VAO = ::asset::VAO;
using VBO = ::asset::VBO;
using RenderCommand = ::scene::RenderCommand;
using Entity = ::scene::Entity;

EnvironmentSystem::EnvironmentSystem(scene::Scene* scene) : System(scene) {}

void EnvironmentSystem::Awake() {

  CORE_INFO("Build IBL...");
  SetIBL();
  CORE_INFO("Complete IBL...");

  CORE_INFO("Build UBO...");
  SetUBO();
  CORE_INFO("Complete UBO...");
}

void EnvironmentSystem::OnUpdateRuntime() {
  // TODO
}

void EnvironmentSystem::OnEditorRumtime(Entity& editor_camera) {
  // 设置 UBO等 全局逻辑 在每帧渲染前 执行
  using CameraFps = ::component::CameraFps;
  using namespace component;

  // 编辑相机案件移动等逻辑
  auto& camera = editor_camera.GetComponent<CameraFps>();
  camera.update();

  // 设置主相机属性
  if (auto ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("CameraData"); true) {
    glm::mat4 proj = editor_camera.GetComponent<CameraFps>().get_projection_mat();
    glm::mat4 view = editor_camera.GetComponent<CameraFps>().get_view_mat();
    glm::vec4 pos = glm::vec4(editor_camera.GetComponent<CameraFps>().T->get_position(), 1.0);
    glm::vec4 forward = glm::vec4(editor_camera.GetComponent<CameraFps>().T->m_forward, 0.0);
    ubo->set_uniform(0, glm::value_ptr(proj));
    ubo->set_uniform(1, glm::value_ptr(view));
    ubo->set_uniform(2, glm::value_ptr(pos));
    ubo->set_uniform(3, glm::value_ptr(forward));

  }

  // 设置平行光
  auto dl_view = m_scene->registry.view<DirectionLight, Transform>();
  for (auto& e : dl_view) {
    auto ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("DL");
    auto& dl = dl_view.get<DirectionLight>(e);
    auto& dt = dl_view.get<Transform>(e);
    auto color = glm::vec4(dl.m_color, 1.0);
    auto directionl = glm::vec4(-dt.m_forward, 0.0);
    auto intensity = dl.m_intensity;
    ubo->set_uniform(0, glm::value_ptr(color));
    ubo->set_uniform(1, glm::value_ptr(directionl));
    ubo->set_uniform(2, &intensity);
  }

  auto pl_view = m_scene->registry.view<PointLight, Transform>();
  for (auto& e : pl_view) {
    auto ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("PL");
    auto& pl = pl_view.get<PointLight>(e);
    auto& pt = pl_view.get<Transform>(e);
    auto color = glm::vec4(pl.m_color, 1.0);
    auto position = glm::vec4(pt.get_position(), 1.0);
    auto intensity = pl.m_intensity;
    auto linear = pl.m_linear;
    auto quadratic = pl.m_quadratic;
    auto range = pl.m_range;

    ubo->set_uniform(0, glm::value_ptr(color));
    ubo->set_uniform(1, glm::value_ptr(position));
    ubo->set_uniform(2, &intensity);
    ubo->set_uniform(3, &linear);
    ubo->set_uniform(4, &quadratic);
    ubo->set_uniform(5, &range);
  }

 
  auto animate_view = m_scene->registry.view<Model, Animator>();
  for (auto& e : animate_view) {
    auto& animator = animate_view.get<Animator>(e);
    auto& model = animate_view.get<Model>(e);
    bool is_update = animator.m_action;
    if (is_update) {
      float tick_speed = animator.m_tick_speed;
      animator.Update(model, Clock::dt * tick_speed);
    }
  }


  // TODO
  // if (auto ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("SL"); true) {
  // auto& camera = main_camera.GetComponent<Transform>();
  // auto& sp = camera_light.GetComponent<Spotlight>();
  // auto color = glm::vec4(sp.m_color, 1.0);
  // auto pos = glm::vec4(camera.get_position(), 1.0);
  // auto direction = glm::vec4(-camera.m_forward, 0.0);
  // float intensity = sp.m_intensity;
  // float inner_cos = sp.get_inner_cosine();
  // float outer_cos = sp.get_outer_cosine();
  // float range = sp.m_range;
  // ubo.set_uniform(0, glm::value_ptr(color));
  // ubo.set_uniform(1, glm::value_ptr(pos));
  // ubo.set_uniform(2, glm::value_ptr(direction));
  // ubo.set_uniform(3, &intensity);
  // ubo.set_uniform(4, &inner_cos);
  // ubo.set_uniform(5, &outer_cos);
  // ubo.set_uniform(6, &range);
  //}

  // 渲染天空盒
  // auto& render_queue = m_scene->render_queue;
  // auto& reg = m_scene->registry;
  // auto mesh_group = reg.group<Mesh>(entt::get<Transform, Material, Tag>);

  // for (auto& e : render_queue) {
  // if (mesh_group.contains(e)) {
  // auto& mesh = mesh_group.get<Mesh>(e);
  // auto& transform = mesh_group.get<Transform>(e);
  // auto& material = mesh_group.get<Material>(e);
  // auto& tag = mesh_group.get<Tag>(e);
  // if (tag.contains(ETag::Skybox)) {
  // material.bind();
  // material.set_uniform("model", transform.get_transform());
  //::scene::Render::set_front_is_ccw(false);
  // mesh.draw();
  //::scene::Render::set_front_is_ccw(true);
  //}
  //}
  //}
}

void EnvironmentSystem::SetUBO() {
  auto camera_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("CameraData");
  auto dl_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("DL");
  auto pl_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("PL");
  auto sl_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("SL");

  CORE_DEBUG("setUBO shader count : {}", m_scene->shader_id_set.size());
  auto& view = m_scene->shader_id_set;
  for (const uint32_t& shader_id : view) {
    // TODO 后续需要优化接口
    camera_ubo->set_binding(0U, "Matrices", shader_id);
    dl_ubo->set_binding(1U, "DL", shader_id);
    pl_ubo->set_binding(2U, "PL", shader_id);
    sl_ubo->set_binding(3U, "SL", shader_id);
  }
}

void EnvironmentSystem::SetIBL() {
  // 生成辐照图
  glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.f);
  glm::mat4 views[] = {
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

  // clang-format off
  // 创建cube
  static float data[] = {
    // back face
    -1.0f, -1.0f, -1.0f, // bottom-left
     1.0f,  1.0f, -1.0f, // top-right
     1.0f, -1.0f, -1.0f, // bottom-right
     1.0f,  1.0f, -1.0f, // top-right
    -1.0f, -1.0f, -1.0f, // bottom-left
    -1.0f,  1.0f, -1.0f, // top-left
    // front face
    -1.0f, -1.0f,  1.0f, // bottom-left
     1.0f, -1.0f,  1.0f, // bottom-right
     1.0f,  1.0f,  1.0f, // top-right
     1.0f,  1.0f,  1.0f, // top-right
    -1.0f,  1.0f,  1.0f, // top-left
    -1.0f, -1.0f,  1.0f, // bottom-left
    // left face
    -1.0f,  1.0f,  1.0f, // top-right
    -1.0f,  1.0f, -1.0f, // top-left
    -1.0f, -1.0f, -1.0f, // bottom-left
    -1.0f, -1.0f, -1.0f, // bottom-left
    -1.0f, -1.0f,  1.0f, // bottom-right
    -1.0f,  1.0f,  1.0f, // top-right
    // right face
     1.0f,  1.0f,  1.0f, // top-left
     1.0f, -1.0f, -1.0f, // bottom-right
     1.0f,  1.0f, -1.0f, // top-right
     1.0f, -1.0f, -1.0f, // bottom-right
     1.0f,  1.0f,  1.0f, // top-left
     1.0f, -1.0f,  1.0f, // bottom-left
    // bottom face
    -1.0f, -1.0f, -1.0f, // top-right
     1.0f, -1.0f, -1.0f, // top-left
     1.0f, -1.0f,  1.0f, // bottom-left
     1.0f, -1.0f,  1.0f, // bottom-left
    -1.0f, -1.0f,  1.0f, // bottom-right
    -1.0f, -1.0f, -1.0f, // top-right
    // top face
    -1.0f,  1.0f, -1.0f, // top-left
     1.0f,  1.0f , 1.0f, // bottom-right
     1.0f,  1.0f, -1.0f, // top-right
     1.0f,  1.0f,  1.0f, // bottom-right
    -1.0f,  1.0f, -1.0f, // top-left
    -1.0f,  1.0f,  1.0f  // bottom-left
  };
  // clang-format on

  auto cube_vbo = std::make_unique<VBO>(sizeof(data), data, GL_STATIC_DRAW);
  auto cube_vao = std::make_unique<VAO>();
  cube_vao->set_vbo(*cube_vbo, 0U, 3U, 3U * sizeof(float), 0, GL_FLOAT);

  auto irradian_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("irradian");

  auto irradian = PublicSingleton<Library<Texture>>::GetInstance().Get("irradian");
  GLuint low_resolution = irradian->Width();
  auto irradian_fbo = std::make_unique<FBO>(low_resolution, low_resolution);

  irradian_shader->bind();
  irradian_shader->set_uniform("projection", proj);
  irradian_shader->set_uniform("texture_0", 0);

  // 将hdr转换成cubemap 通过FBO将6个方向的渲染画面写入cubemap
  glViewport(0, 0, low_resolution, low_resolution);
  irradian_fbo->bind();

  auto skybox_hdr_texutre = PublicSingleton<Library<Texture>>::GetInstance().Get("hdr");
  skybox_hdr_texutre->bind(0);
  int faces = 6;
  for (int i = 0; i < faces; i++) {
    irradian_shader->set_uniform("view", views[i]);
    irradian_fbo->set_color_texture(0, irradian->get_id(), i);
    RenderCommand::clear_buffer();
    cube_vao->draw(0U, 36U);
  }
  irradian_fbo->ubind();

  auto prefiltermap = PublicSingleton<Library<Texture>>::GetInstance().Get("prefiltermap");
  auto prefilter_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("prefiler");

  auto prefilter_fbo = std::make_shared<FBO>(low_resolution, low_resolution);

  prefilter_fbo->set_depth_texture();

  prefilter_shader->bind();
  prefilter_shader->set_uniform("projection", proj);

  skybox_hdr_texutre->bind(0);
  prefilter_fbo->bind();

  const int kMaxMipmap = 5;

  for (int mip = 0; mip < kMaxMipmap; mip++) {
    GLuint mip_width = low_resolution * std::pow(0.5, mip);
    GLuint mip_height = low_resolution * std::pow(0.5, mip);

    prefilter_fbo->reset_depth_texture(mip_width, mip_height);
    glViewport(0, 0, mip_width, mip_height);
    float roughness = static_cast<float>(mip) / (kMaxMipmap - 1);
    prefilter_shader->set_uniform("roughness", roughness);
    for (int face = 0; face < faces; face++) {
      prefilter_shader->set_uniform("view", views[face]);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                             prefiltermap->get_id(), mip);
      RenderCommand::clear_buffer();

      cube_vao->draw(0, 36);
    }
  }
  prefilter_fbo->ubind();

  // clang-format off
  GLfloat quad_data[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
  };
  // clang-format on

  auto quad_vbo = std::make_shared<VBO>(sizeof(quad_data), quad_data, GL_STATIC_DRAW);
  auto quad_vao = std::make_shared<VAO>();
  quad_vao->set_vbo(*quad_vbo, 0, 3, 5 * sizeof(GLfloat), 0, GL_FLOAT);
  quad_vao->set_vbo(*quad_vbo, 1, 2, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat), GL_FLOAT);

  auto BRDF_LUT_texture = PublicSingleton<Library<Texture>>::GetInstance().Get("BRDF_LUT");
  constexpr GLuint kResolution = 512U;
  auto BRDF_LUT_fbo = std::make_shared<FBO>(kResolution, kResolution);

  auto BRDF_LUT_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("BRDF_LUT");
  BRDF_LUT_fbo->set_depth_texture();
  BRDF_LUT_fbo->bind();
  BRDF_LUT_shader->bind();

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BRDF_LUT_texture->get_id(), 0);
  glViewport(0, 0, kResolution, kResolution);

  RenderCommand::clear_buffer();

  quad_vao->draw(0, 6);

  BRDF_LUT_fbo->ubind();

  CHECK_ERROR();
}

}  // namespace system
}  // namespace saber
