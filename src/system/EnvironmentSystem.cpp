#include <system/EnvironmentSystem.h>
#include <asset/Buffer.h>
#include <asset/FBO.h>
#include <library/ShaderLibrary.h>
#include <library/TextureLibrary.h>
#include <scene/Render.h>
#include <scene/Scene.h>
namespace saber {
namespace system {

using Texture = asset::Texture;
using FBO = ::asset::FBO;
using VAO = ::asset::VAO;
using VBO = ::asset::VBO;
using Render = ::scene::Render;

EnvironmentSystem::EnvironmentSystem(scene::Scene* scene) : System(scene) {}

void EnvironmentSystem::Awake() { SetIBL(); }

void EnvironmentSystem::OnUpdateRuntime() {
  // TODO
}

void EnvironmentSystem::OnEditorRumtime() {


  // 渲染天空盒
  using namespace component;
  auto& render_queue = m_scene->render_queue;
  auto& reg = m_scene->registry;
  auto mesh_group = reg.group<Mesh>(entt::get<Transform, Material, Tag>);

  for (auto& e : render_queue) {
    if(mesh_group.contains(e)) {
      auto& mesh = mesh_group.get<Mesh>(e);
      auto& transform = mesh_group.get<Transform>(e);
      auto& material = mesh_group.get<Material>(e);
      auto& tag = mesh_group.get<Tag>(e);
      if (tag.contains(ETag::Skybox)) {
        material.bind();
        material.set_uniform("model", transform.get_transform());

        ::scene::Render::set_front_is_ccw(false);
        mesh.draw();
        ::scene::Render::set_front_is_ccw(true);
      }
    }
  }
}

void EnvironmentSystem::SetUBO() {

  auto camera_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("CameraData");
  auto dl_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("DL");
  auto pl_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("PL");
  auto sl_ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("SL");

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
  auto irradian =
      PublicSingleton<Library<Texture>>::GetInstance().Get("irradian");
  GLuint low_resolution = irradian->Width();
  auto irradian_fbo = std::make_unique<FBO>(low_resolution, low_resolution);

  glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.f);

  glm::mat4 views[] = {
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, -1.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f)),
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
                  glm::vec3(0.0f, -1.0f, 0.0f))};

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

  // auto irradian_shader = std::make_unique<Shader>(
  //    "../resource/shader/irradian.vs", "../resource/shader/irradian.fs");
  auto irradian_shader =
      PublicSingleton<Library<Shader>>::GetInstance().Get("irradian");

  irradian_shader->bind();
  irradian_shader->set_uniform("projection", proj);
  irradian_shader->set_uniform("texture_0", 0);

  // 将hdr转换成cubemap 通过FBO将6个方向的渲染画面写入cubemap
  glViewport(0, 0, low_resolution, low_resolution);
  irradian_fbo->bind();

  auto skybox_hdr_texutre =
      PublicSingleton<Library<Texture>>::GetInstance().Get("hdr");
  skybox_hdr_texutre->bind(0);
  int faces = 6;
  for (size_t i = 0; i < faces; i++) {
    irradian_shader->set_uniform("view", views[i]);
    irradian_fbo->set_color_texture(0, irradian->get_id(), i);
    Render::clear_buffer();
    cube_vao->draw(0U, 36U);
  }
  irradian_fbo->ubind();

  // prefiltermap = std::make_shared<Texture>(
  //     GL_TEXTURE_CUBE_MAP, low_resolution, low_resolution, GL_FLOAT, 5);
  auto prefiltermap =
      PublicSingleton<Library<Texture>>::GetInstance().Get("prefiltermap");
  auto prefilter_shader = std::make_shared<Shader>(
      "../resource/shader/prefiler.vs", "../resource/shader/prefilter.fs");
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
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                             prefiltermap->get_id(), mip);
      Render::clear_buffer();

      cube_vao->draw(0, 36);
    }
  }
  prefilter_fbo->ubind();

  // clnag-format off
  GLfloat quad_data[] = {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f,
                         1.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f,
                         -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  1.0f,
                         1.0f,  -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};
  // clang-format on

  auto quad_vbo =
      std::make_shared<VBO>(sizeof(quad_data), quad_data, GL_STATIC_DRAW);
  auto quad_vao = std::make_shared<VAO>();
  quad_vao->set_vbo(*quad_vbo, 0, 3, 5 * sizeof(GLfloat), 0, GL_FLOAT);
  quad_vao->set_vbo(*quad_vbo, 1, 2, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat),
                    GL_FLOAT);

  auto BRDF_LUT_texture =
      PublicSingleton<Library<Texture>>::GetInstance().Get("irradian");
  constexpr GLuint kResolution = 512U;
  auto BRDF_LUT_fbo = std::make_shared<FBO>(kResolution, kResolution);
  // auto BRDF_LUT_shader = std::make_shared<Shader>(
  //    "../resource/shader/BRDF_LUT.vs", "../resource/shader/BRDF_LUT.fs");
  auto BRDF_LUT_shader =
      PublicSingleton<Library<Shader>>::GetInstance().Get("BRDF_LUT");

  BRDF_LUT_fbo->set_depth_texture();

  BRDF_LUT_fbo->bind();
  BRDF_LUT_shader->bind();

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         BRDF_LUT_texture->get_id(), 0);

  glViewport(0, 0, kResolution, kResolution);

  Render::clear_buffer();

  quad_vao->draw(0, 6);

  BRDF_LUT_fbo->ubind();

  CHECK_ERROR();
}

}  // namespace system
}  // namespace saber
