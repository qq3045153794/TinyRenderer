#include <component/Light.h>
#include <core/Window.h>
#include <editor/EditorLayer.h>
#include <imGui/ImGuiWrapper.h>
#include <library/ShaderLibrary.h>
#include <library/TextureLibrary.h>
#include <scene/RenderCommand.h>

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
  m_hierarchy_panel = std::make_unique<SceneHierarchyPanel>(m_cur_scene);
  m_content_brower_panel = std::make_unique<ContentBrowerPanel>();

  ImGuiWrapper::Init();

  // 创建 Entity
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
  quad.AddComponent<Material>(Material::ShadingModel::DEFAULT);
  quad.GetComponent<Material>().set_texture(0, brdf_lut_texture);
  quad.GetComponent<Transform>().set_position(glm::vec3(-4, 0.0, 0.0));
  CORE_INFO("{} created", quad.name);

  Entity cube = scene->create_entity("cube");
  cube.AddComponent<Mesh>(Mesh::primitive::CUBE);
  cube.AddComponent<Material>(Material::ShadingModel::DEFAULT);
  cube.GetComponent<Material>().set_texture(0, default_texture);
  cube.GetComponent<Transform>().translate(glm::vec3(8.0, 0.0, 0.0));
  CORE_INFO("{} created", cube.name);

  Entity sphere = scene->create_entity("sphere");
  sphere.AddComponent<Mesh>(Mesh::primitive::SPHERE);
  sphere.GetComponent<Transform>().translate(glm::vec3(4.0, 0.0, 0.0));
  sphere.AddComponent<Material>(Material::ShadingModel::PBR);
  auto& sphere_mat = sphere.GetComponent<Material>();
  sphere_mat.bind_texture(Material::pbr_t::irradiance_map, irradian_texture);
  sphere_mat.bind_texture(Material::pbr_t::prefilter_map, prefiltermap);
  sphere_mat.bind_texture(Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
  sphere_mat.bind_uniform(Material::pbr_u::albedo, sphere_albedo);
  sphere_mat.bind_uniform(Material::pbr_u::metalness, sphere_metalness);
  sphere_mat.bind_uniform(Material::pbr_u::roughness, sphere_roughness);
  sphere_mat.bind_uniform(Material::pbr_u::ao, sphere_ao);
  CHECK_ERROR();
  CORE_INFO("{} created", sphere.name);

  Entity sphere_pbr = scene->create_entity("sphere pbr");
  sphere_pbr.AddComponent<Mesh>(Mesh::primitive::SPHERE);
  sphere_pbr.GetComponent<Transform>().translate(glm::vec3(4.0, 4.0, 0.0));
  sphere_pbr.AddComponent<Material>(Material::ShadingModel::PBR);
  auto& sphere_mat_pbr = sphere_pbr.GetComponent<Material>();
  sphere_mat_pbr.bind_texture(Material::pbr_t::irradiance_map, irradian_texture);
  sphere_mat_pbr.bind_texture(Material::pbr_t::prefilter_map, prefiltermap);
  sphere_mat_pbr.bind_texture(Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
  auto albedo_pbr = std::make_shared<::asset::Texture>(
      "/home/xietao/git/TinyRenderer/resource/texture/pbr_0/MetalPlates013_2K_Color.jpg");
  sphere_mat_pbr.bind_texture(Material::pbr_t::albedo, albedo_pbr);
  auto normal_pbr = std::make_shared<::asset::Texture>(
      "/home/xietao/git/TinyRenderer/resource/texture/pbr_0/MetalPlates013_2K_NormalGL.jpg");
  sphere_mat_pbr.bind_texture(Material::pbr_t::normal, normal_pbr);
  auto metalness_pbr = std::make_shared<::asset::Texture>(
      "/home/xietao/git/TinyRenderer/resource/texture/pbr_0/MetalPlates013_2K_Metalness.jpg");
  sphere_mat_pbr.bind_texture(Material::pbr_t::metalness, metalness_pbr);
  auto roughness_pbr = std::make_shared<::asset::Texture>(
      "/home/xietao/git/TinyRenderer/resource/texture/pbr_0/MetalPlates013_2K_Roughness.jpg");
  sphere_mat_pbr.bind_texture(Material::pbr_t::roughness, roughness_pbr);
  auto ao_pbr = std::make_shared<::asset::Texture>(
      "/home/xietao/git/TinyRenderer/resource/texture/pbr_0/MetalPlates013_2K_AmbientOcclusion.jpg");
  sphere_mat_pbr.bind_texture(Material::pbr_t::ao, ao_pbr);
  CHECK_ERROR();
  CORE_INFO("{} created", sphere_pbr.name);

  m_editor_camera = scene->create_entity("main_camera");
  m_editor_camera.AddComponent<CameraFps>(
      60.f, static_cast<float>(core::Window::m_width) / static_cast<float>(core::Window::m_height), 0.1f, 100.f);
  m_editor_camera.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 5.0));
  CHECK_ERROR();
  CORE_INFO("{} created", sphere.name);

  Entity paimon = scene->create_entity("paimon");
  paimon.AddComponent<Model>("../resource/objects/paimon/untitled.obj", Quality::Auto);
  paimon.GetComponent<Transform>().set_position(glm::vec3(0.0, -2.0, 0.0));
  paimon.GetComponent<Transform>().set_scale(glm::vec3(4.0, 4.0, 4.0));
  auto paimon_1 = std::make_shared<Material>(default_shader);
  auto& temp_mat_1 = paimon.GetComponent<Model>().SetMatermial("披风", *paimon_1);
  temp_mat_1.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/披风2.jpg", false, 5));
  auto& temp_mat_2 = paimon.GetComponent<Model>().SetMatermial("头发", *paimon_1);
  temp_mat_2.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/头发.jpg", false, 5));
  auto& temp_mat_3 = paimon.GetComponent<Model>().SetMatermial("衣服", *paimon_1);
  temp_mat_3.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/衣服.jpg", false, 5));
  auto& temp_mat_4 = paimon.GetComponent<Model>().SetMatermial("皮肤", *paimon_1);
  temp_mat_4.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/衣服.jpg", false, 5));
  auto& temp_mat_5 = paimon.GetComponent<Model>().SetMatermial("眼睛", *paimon_1);
  temp_mat_5.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/头发.jpg", false, 5));
  auto& temp_mat_6 = paimon.GetComponent<Model>().SetMatermial("表情", *paimon_1);
  temp_mat_6.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/表情.png", false, 5));
  auto& temp_mat_7 = paimon.GetComponent<Model>().SetMatermial("脸", *paimon_1);
  temp_mat_7.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/脸.jpg", false, 5));
  auto& temp_mat_8 = paimon.GetComponent<Model>().SetMatermial("眼白", *paimon_1);
  temp_mat_8.set_texture(0, std::make_shared<asset::Texture>("../resource/objects/paimon/Texture/脸.jpg", false, 5));
  CHECK_ERROR();

  // 展示设置 800 600
  main_fbo = std::make_shared<asset::FBO>(1024, 576);
  main_fbo->set_color_texture();
  main_fbo->set_depth_texture();

  scene->SubmitRender(quad.id);
  scene->SubmitRender(cube.id);
  scene->SubmitRender(sphere.id);
  scene->SubmitRender(sphere_pbr.id);
  scene->SubmitRender(paimon.id);
  // 最后渲染
  scene->SubmitRender(skybox.id);
}

void EditorLayer::Awake() { m_cur_scene->Awake(); }
void EditorLayer::OnDetach() {
  // TODO
}
void EditorLayer::OnUpdateRuntime() {
  // glfwSetInputMode(::core::Window::m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  main_fbo->bind();
  glViewport(0U, 0U, 1026, 576);
  ::scene::RenderCommand::clear_buffer();
  m_cur_scene->OnEditorRumtime(m_editor_camera);
  main_fbo->ubind();

}
void EditorLayer::OnImGuiRender() {
  // glViewport(0U, 0U, 800, 600);
  ::scene::RenderCommand::clear_buffer();
  static bool dockspaceOpen = true;
  // static bool viewportOpen = true;
  TriggerViewPort();

  ImGuiWrapper::Begin();
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);


  ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
  window_flags |=
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  ImGui::Begin("test", &dockspaceOpen, window_flags);
  // 设置BeginMainMenuBar才不会docking遮盖
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();

      if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenScene();

      if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) SaveScene();

      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  static bool hirerchy_panel_open = true;
  m_hierarchy_panel->OnImGuiRender(&hirerchy_panel_open);

  m_content_brower_panel->OnImGuiRender();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  // ImGui::SetNextWindowPos(ImVec2{0, 30});
  ImGui::SetNextWindowSize(ImVec2{static_cast<float>(main_fbo->Width()), static_cast<float>(main_fbo->Height())});
  // static ImGuiWindowFlags viewport_window_flags =
  //    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
  if (ImGui::Begin("ViewPort")) {
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();
    bound_viewport[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
    bound_viewport[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

    ImVec2 viewportPanelSize = {static_cast<float>(main_fbo->Width()), static_cast<float>(main_fbo->Height())};
    uint32_t texture_id = main_fbo->get_color_texture().get_id();
    ImGui::Image((void*)(intptr_t)texture_id, viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
    ImGui::End();
    ImGui::PopStyleVar();
  }
  ImGui::End();

  ImGuiWrapper::End();
  // TODO
}

void EditorLayer::NewScene() {
  // TODO
}

void EditorLayer::OpenScene() {
  // TODO
}

void EditorLayer::SaveScene() {
  // TODO
}

void EditorLayer::TriggerViewPort() {
  // 未解除不能隐藏鼠标问题 初步判断和平台有问题
  ::core::Window::layer = ::core::Layer::ImGui;
  auto mouse_pos = ImGui::GetMousePos();
  bool is_pressed = ImGui::IsMouseDown(2);
  static bool is_first_pressed = true;
  if (mouse_pos.x >= bound_viewport[0].x && mouse_pos.x <= bound_viewport[1].x && mouse_pos.y >= bound_viewport[0].y &&
      mouse_pos.y <= bound_viewport[1].y && is_pressed) {
    ::core::Window::layer = ::core::Layer::OnViewPort;

    if (is_first_pressed) {
      ::core::Window::bound_viewport_x = mouse_pos.x;
      ::core::Window::bound_viewport_y = mouse_pos.y;
      is_first_pressed = false;
    }
  }else {
    is_first_pressed = true;
  }

}


void EditorLayer::OnEvent() {}

void EditorLayer::CreateScene() {}

}  // namespace editor
}  // namespace saber
