#include <component/Light.h>
#include <core/Window.h>
#include <editor/EditorLayer.h>
#include <library/ShaderLibrary.h>
#include <library/TextureLibrary.h>
#include <scene/Render.h>

#include <scene/Entity.hpp>
namespace saber {
namespace editor {

static glm::vec4 sphere_albedo{0.22f, 0.0f, 1.0f, 1.0f};
static float sphere_metalness = 0.05f;
static float sphere_roughness = 0.2f;
static float sphere_ao = 1.0f;

EditorLayer::EditorLayer() {}

void EditorLayer::OnAttach() {
  using namespace ::component;
  using Entity = ::scene::Entity;

  m_cur_scene = std::make_shared<scene::Scene>("hello world");

  auto& scene = m_cur_scene;
  Entity sun_light = scene->create_entity("sum light");
  sun_light.AddComponent<DirectionLight>(glm::vec3(1.0, 1.0, 1.0), 1.0);
  sun_light.GetComponent<Transform>().set_ealar_YZX(glm::vec3(-45, 0.0, 0.0));

  Entity point_light = scene->create_entity("point light");
  point_light.AddComponent<PointLight>(glm::vec3(1.0, 1.0, 1.0), 1.0);
  point_light.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, -5.0));
  point_light.GetComponent<PointLight>().set_attenuation(0.09f, 0.032f);

  auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
  scene->registry_shader(default_shader->get_id());
  auto skybox_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("skybox");
  scene->registry_shader(skybox_shader->get_id());
  auto pbr_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("pbr");
  scene->registry_shader(pbr_shader->get_id());

  auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
  auto brdf_lut_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("BRDF_LUT");
  auto irradian_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("irradian");
  auto prefiltermap = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("prefiltermap");
  auto skybox_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("hdr");

  Entity skybox = scene->create_entity("skybox", ETag::Skybox);
  skybox.AddComponent<Mesh>(Mesh::primitive::CUBE);
  skybox.AddComponent<Material>(skybox_shader);
  skybox.GetComponent<Material>().set_texture(0, skybox_texture);

  Entity quad = scene->create_entity("quad");
  quad.AddComponent<Mesh>(Mesh::primitive::QUAD);
  quad.AddComponent<Material>(default_shader);
  quad.GetComponent<Material>().set_texture(0, brdf_lut_texture);
  quad.GetComponent<Transform>().set_position(glm::vec3(-4, 0.0, 0.0));
  CORE_INFO("{} created", quad.name);

  Entity cube = scene->create_entity("cube");
  cube.AddComponent<Mesh>(Mesh::primitive::CUBE);
  cube.AddComponent<Material>(default_shader);
  cube.GetComponent<Material>().set_texture(0, default_texture);
  cube.GetComponent<Transform>().translate(glm::vec3(8.0, 0.0, 0.0));
  CORE_INFO("{} created", cube.name);

  Entity sphere = scene->create_entity("sphere");
  sphere.AddComponent<Mesh>(Mesh::primitive::SPHERE);
  sphere.GetComponent<Transform>().translate(glm::vec3(4.0, 0.0, 0.0));
  sphere.AddComponent<Material>(pbr_shader);
  auto& sphere_mat = sphere.GetComponent<Material>();
  sphere_mat.set_texture(Material::pbr_t::irradiance_map, irradian_texture);
  sphere_mat.set_texture(Material::pbr_t::prefilter_map, prefiltermap);
  sphere_mat.set_texture(Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
  sphere_mat.bing_uniform(Material::pbr_u::albedo, sphere_albedo);
  sphere_mat.bing_uniform(Material::pbr_u::metalness, sphere_metalness);
  sphere_mat.bing_uniform(Material::pbr_u::roughness, sphere_roughness);
  sphere_mat.bing_uniform(Material::pbr_u::ao, sphere_ao);
  CHECK_ERROR();
  CORE_INFO("{} created", sphere.name);

  m_editor_camera = scene->create_entity("main_camera");
  m_editor_camera.AddComponent<CameraFps>(
      60.f, static_cast<float>(core::Window::m_width) / static_cast<float>(core::Window::m_height), 0.1f, 100.f);
  m_editor_camera.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 5.0));
  CHECK_ERROR();

  main_fbo = std::make_shared<asset::FBO>(core::Window::m_width, core::Window::m_height);
  main_fbo->set_color_texture();
  main_fbo->set_depth_texture();

  scene->SubmitRender(skybox.id);
  scene->SubmitRender(quad.id);
  scene->SubmitRender(cube.id);
  scene->SubmitRender(sphere.id);
}

void EditorLayer::Awake() {
  m_cur_scene->Awake();
  ::scene::Render::eable_depth_test(true);
  ::scene::Render::eable_alpha_blend(true);
  ::scene::Render::eable_face_culling(true);
  ::core::Window::resize();
}
void EditorLayer::OnDetach() {
  // TODO
}
void EditorLayer::OnUpdateRuntime() {
  main_fbo->bind();
  ::scene::Render::clear_buffer();
  m_cur_scene->OnEditorRumtime(m_editor_camera);
  main_fbo->ubind();
}
void EditorLayer::OnImGuiRender() {
  glViewport(0U, 0U, core::Window::m_width, core::Window::m_height);
  ::scene::Render::clear_buffer();
  main_fbo->draw();
  // TODO
}

void EditorLayer::OnEvent() {}

void EditorLayer::CreateScene() {}

}  // namespace editor
}  // namespace saber
