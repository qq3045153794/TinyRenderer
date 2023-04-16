#include <component/Light.h>
#include <core/Input.h>
#include <core/Window.h>
#include <editor/EditorLayer.h>
#include <imGui/ImGuiWrapper.h>
#include <library/ShaderLibrary.h>
#include <library/TextureLibrary.h>
#include <manage/ConfigManage.h>
#include <scene/RenderCommand.h>
#include <scene/SerializeEntity.h>

#include <scene/Entity.hpp>
namespace saber {
namespace editor {

static glm::vec4 sphere_albedo{0.22f, 0.0f, 1.0f, 1.0f};
static float sphere_metalness = 0.05f;
static float sphere_roughness = 0.2f;
static float sphere_ao = 1.0f;

EditorLayer::EditorLayer() {}

::scene::Entity EditorLayer::create_entity(const std::string& name, component::ETag tag) {
  ::scene::Entity e = {name, registry.create(), &registry};
  e.AddComponent<component::Transform>();
  e.AddComponent<component::Tag>(tag, name);
  return e;
}

void EditorLayer::OnAttach() {
  using namespace ::component;
  using Entity = ::scene::Entity;
  m_cur_scene = std::make_shared<scene::Scene>("hello world");
  m_hierarchy_panel = std::make_unique<SceneHierarchyPanel>(m_cur_scene);
  m_content_brower_panel = std::make_unique<ContentBrowerPanel>();

  ImGuiWrapper::Init();

  // 创建 Entity
  auto& scene = m_cur_scene;
#if 0
/*
  Entity sun_light = scene->create_entity("sum light", ::component::ETag::Lamplight);
  sun_light.AddComponent<DirectionLight>(glm::vec3(1.0, 1.0, 1.0), 1.0);
  sun_light.GetComponent<Transform>().set_ealar_YZX(glm::vec3(-45, 0.0, 0.0));
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / sun_light.name, sun_light);
*/
  ::scene::SerializeObject::DeserializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / "sum light", *scene);
  CHECK_ERROR();
/*
  Entity point_light = scene->create_entity("point light", ::component::ETag::Lamplight);
  point_light.AddComponent<PointLight>(glm::vec3(1.0, 1.0, 1.0), 1.0);
  point_light.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, -5.0));
  point_light.GetComponent<PointLight>().set_attenuation(0.09f, 0.032f);
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / point_light.name, point_light);
*/
  ::scene::SerializeObject::DeserializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / "point light", *scene);
  CHECK_ERROR();

  auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
  scene->registry_shader(default_shader->get_id());
  auto skybox_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("skybox");
  scene->registry_shader(skybox_shader->get_id());
  auto pbr_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("pbr");
  scene->registry_shader(pbr_shader->get_id());
  CHECK_ERROR();

  auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
  auto brdf_lut_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("BRDF_LUT");
  auto irradian_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("irradian");
  auto prefiltermap = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("prefiltermap");
  auto skybox_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("hdr");
  CHECK_ERROR();
/*
  Entity skybox = scene->create_entity("skybox", ETag::Skybox);
  skybox.AddComponent<Mesh>(Mesh::primitive::CUBE);
  skybox.AddComponent<Material>(skybox_shader);
  skybox.GetComponent<Material>().set_texture(0, skybox_texture);
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / skybox.name, skybox);
*/

  auto skybox = ::scene::SerializeObject::DeserializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / "skybox", *scene);
  CHECK_ERROR();
/*
  Entity quad = scene->create_entity("quad");
  quad.AddComponent<Mesh>(Mesh::primitive::QUAD);
  quad.AddComponent<Material>(Material::ShadingModel::DEFAULT);
  quad.GetComponent<Material>().set_texture(0, brdf_lut_texture);
  quad.GetComponent<Transform>().set_position(glm::vec3(-4, 0.0, 0.0));
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / quad.name, quad);
  CHECK_ERROR();
  CORE_INFO("{} created", quad.name);
*/
/*
  Entity cube = scene->create_entity("cube");
  cube.AddComponent<Mesh>(Mesh::primitive::CUBE);
  cube.AddComponent<Material>(Material::ShadingModel::DEFAULT);
  cube.GetComponent<Material>().set_texture(0, default_texture);
  cube.GetComponent<Transform>().translate(glm::vec3(8.0, 0.0, 0.0));
  // 序列化
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / cube.name, cube);
  CHECK_ERROR();
*/
  auto cube = ::scene::SerializeObject::DeserializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / "cube", *scene);
  CORE_INFO("{} created", cube.name);

/*
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
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / sphere.name, sphere);
  CHECK_ERROR();
*/

  auto sphere = ::scene::SerializeObject::DeserializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / "sphere", *scene);
  CORE_INFO("{} created", sphere.name);

/*
  Entity sphere_pbr = scene->create_entity("sphere pbr");
  sphere_pbr.AddComponent<Mesh>(Mesh::primitive::SPHERE);
  sphere_pbr.GetComponent<Transform>().translate(glm::vec3(4.0, 4.0, 0.0));
  sphere_pbr.AddComponent<Material>(Material::ShadingModel::PBR);
  auto& sphere_mat_pbr = sphere_pbr.GetComponent<Material>();
  sphere_mat_pbr.bind_texture(Material::pbr_t::irradiance_map, irradian_texture);
  sphere_mat_pbr.bind_texture(Material::pbr_t::prefilter_map, prefiltermap);
  sphere_mat_pbr.bind_texture(Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
  auto content_path = PublicSingleton<ConfigManage>::GetInstance().content_path / "textures";
  auto albedo_pbr = std::make_shared<::asset::Texture>((content_path / "pbr_0/MetalPlates013_2K_Color.jpg").c_str());
  sphere_mat_pbr.bind_texture(Material::pbr_t::albedo, albedo_pbr);
  auto normal_pbr = std::make_shared<::asset::Texture>((content_path / "pbr_0/MetalPlates013_2K_NormalGL.jpg").c_str());
  sphere_mat_pbr.bind_texture(Material::pbr_t::normal, normal_pbr);
  auto metalness_pbr = std::make_shared<::asset::Texture>((content_path / "pbr_0/MetalPlates013_2K_Metalness.jpg").c_str());
  sphere_mat_pbr.bind_texture(Material::pbr_t::metalness, metalness_pbr);
  auto roughness_pbr = std::make_shared<::asset::Texture>((content_path / "pbr_0/MetalPlates013_2K_Roughness.jpg").c_str());
  sphere_mat_pbr.bind_texture(Material::pbr_t::roughness, roughness_pbr);
  auto ao_pbr = std::make_shared<::asset::Texture>((content_path / "pbr_0/MetalPlates013_2K_AmbientOcclusion.jpg").c_str());
  sphere_mat_pbr.bind_texture(Material::pbr_t::ao, ao_pbr);
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / sphere_pbr.name, sphere_pbr);
  CHECK_ERROR();
*/

  auto sphere_pbr = ::scene::SerializeObject::DeserializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / "sphere pbr", *scene);
  CORE_INFO("{} created", sphere_pbr.name);

/*
  m_editor_camera = scene->create_entity("main_camera");
  m_editor_camera.AddComponent<CameraFps>(
      60.f, static_cast<float>(core::Window::m_width) / static_cast<float>(core::Window::m_height), 0.1f, 100.f);
  m_editor_camera.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 5.0));
  ::scene::SerializeObject::SerializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / m_editor_camera.name, m_editor_camera);

  m_editor_camera = ::scene::SerializeObject::DeserializeEntity(PublicSingleton<ConfigManage>::GetInstance().content_path / "main_camera", *scene);
  CHECK_ERROR();
  CORE_INFO("{} created", m_editor_camera.name);
*/

/*
  Entity paimon = scene->create_entity("paimon");
  auto paimon_path = PublicSingleton<ConfigManage>::GetInstance().content_path / "models/paimon" ;
  paimon.AddComponent<Model>((paimon_path / "untitled.obj").c_str(), Quality::Auto);
  paimon.GetComponent<Transform>().set_position(glm::vec3(0.0, -2.0, 0.0));
  paimon.GetComponent<Transform>().set_scale(glm::vec3(4.0, 4.0, 4.0));
  auto paimon_1 = std::make_shared<Material>(default_shader);
  auto& temp_mat_1 = paimon.GetComponent<Model>().SetMatermial("披风", *paimon_1);
  temp_mat_1.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/披风2.jpg").c_str(), false, 5));
  auto& temp_mat_2 = paimon.GetComponent<Model>().SetMatermial("头发", *paimon_1);
  temp_mat_2.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/头发.jpg").c_str(), false, 5));
  auto& temp_mat_3 = paimon.GetComponent<Model>().SetMatermial("衣服", *paimon_1);
  temp_mat_3.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/衣服.jpg").c_str(), false, 5));
  auto& temp_mat_4 = paimon.GetComponent<Model>().SetMatermial("皮肤", *paimon_1);
  temp_mat_4.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/衣服.jpg").c_str(), false, 5));
  auto& temp_mat_5 = paimon.GetComponent<Model>().SetMatermial("眼睛", *paimon_1);
  temp_mat_5.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/头发.jpg").c_str(), false, 5));
  auto& temp_mat_6 = paimon.GetComponent<Model>().SetMatermial("表情", *paimon_1);
  temp_mat_6.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/表情.png").c_str(), false, 5));
  auto& temp_mat_7 = paimon.GetComponent<Model>().SetMatermial("脸", *paimon_1);
  temp_mat_7.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/脸.jpg").c_str(), false, 5));
  auto& temp_mat_8 = paimon.GetComponent<Model>().SetMatermial("眼白", *paimon_1);
  temp_mat_8.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/脸.jpg").c_str(), false, 5));
  CHECK_ERROR();
*/
#endif

  // 着色器与UBO绑定
  auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
  scene->registry_shader(default_shader->get_id());
  auto skybox_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("skybox");
  scene->registry_shader(skybox_shader->get_id());
  auto pbr_shader = PublicSingleton<Library<Shader>>::GetInstance().Get("pbr");
  scene->registry_shader(pbr_shader->get_id());

  m_cur_scene_name = "Unnamed";

  // m_editor_camera = scene->create_entity("editor camera", ::component::ETag::MainCamera);
  // m_editor_camera.AddComponent<CameraFps>(
  // 60.f, static_cast<float>(core::Window::m_width) / static_cast<float>(core::Window::m_height), 0.1f, 100.f);
  // m_editor_camera.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 5.0));
  // editor camera 不序列化
  // scene->exclude_entity(m_editor_camera.id);
  // CHECK_ERROR();
  // CORE_INFO("{} created", m_editor_camera.name);

  m_editor_camera = this->create_entity("editor camera", ::component::ETag::MainCamera);
  m_editor_camera.AddComponent<CameraFps>(
      60.f, static_cast<float>(core::Window::m_width) / static_cast<float>(core::Window::m_height), 0.1f, 100.f);
  m_editor_camera.GetComponent<Transform>().set_position(glm::vec3(0.0, 0.0, 5.0));

  auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
/*
  Entity paimon = scene->create_entity("paimon");
  auto paimon_path = PublicSingleton<ConfigManage>::GetInstance().content_path / "models/paimon" ;
  paimon.AddComponent<Model>((paimon_path / "untitled.obj").c_str(), Quality::Auto);
  paimon.GetComponent<Transform>().set_position(glm::vec3(0.0, -2.0, 0.0));
  paimon.GetComponent<Transform>().set_scale(glm::vec3(4.0, 4.0, 4.0));
  auto& paimon_model = paimon.GetComponent<Model>();
  auto paimon_1 = std::make_shared<Material>(Material::ShadingModel::DEFAULT);
  for(auto& [texture_name, uid] : paimon_model.materials_cache) {
    auto& temp_mat = paimon_model.SetMatermial(texture_name, *paimon_1);
    temp_mat.set_texture(0, default_texture);
  }
*/
  Entity aili = scene->create_entity("aili");
  auto aili_path = PublicSingleton<ConfigManage>::GetInstance().content_path / "models/babala/babala.fbx";
  aili.AddComponent<Model>(aili_path.c_str(), Quality::Auto, true);
  auto& aili_model = aili.GetComponent<Model>();
  aili_model.AttachMotion(aili_path);
  auto& animator = aili.AddComponent<Animator>(&aili_model);
  animator.Update(aili.GetComponent<Model>(), Clock::dt);
  // CORE_DEBUG("dt : {}", Clock::dt);
  auto aili_mat = std::make_shared<Material>(Material::ShadingModel::DEFAULT);
  for (auto& [texture_name, uid] : aili_model.materials_cache) {
    auto& temp_mat = aili_model.SetMatermial(texture_name, *aili_mat);
    temp_mat.set_texture(0, default_texture);
    auto& bone_transforms = animator.m_bone_transforms;
    temp_mat.set_bound_arrary("bone_transform", 0U, &bone_transforms);
     // temp_mat.set_uniform();
  }

  /*
  auto& temp_mat_1 = paimon.GetComponent<Model>().SetMatermial("披风", *paimon_1);
  temp_mat_1.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/披风2.jpg").c_str(), false, 5));
  auto& temp_mat_2 = paimon.GetComponent<Model>().SetMatermial("头发", *paimon_1);
  temp_mat_2.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/头发.jpg").c_str(), false, 5));
  auto& temp_mat_3 = paimon.GetComponent<Model>().SetMatermial("衣服", *paimon_1);
  temp_mat_3.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/衣服.jpg").c_str(), false, 5));
  auto& temp_mat_4 = paimon.GetComponent<Model>().SetMatermial("皮肤", *paimon_1);
  temp_mat_4.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/衣服.jpg").c_str(), false, 5));
  auto& temp_mat_5 = paimon.GetComponent<Model>().SetMatermial("眼睛", *paimon_1);
  temp_mat_5.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/头发.jpg").c_str(), false, 5));
  auto& temp_mat_6 = paimon.GetComponent<Model>().SetMatermial("表情", *paimon_1);
  temp_mat_6.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/表情.png").c_str(), false, 5));
  auto& temp_mat_7 = paimon.GetComponent<Model>().SetMatermial("脸", *paimon_1);
  temp_mat_7.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/脸.jpg").c_str(), false, 5));
  auto& temp_mat_8 = paimon.GetComponent<Model>().SetMatermial("眼白", *paimon_1);
  temp_mat_8.set_texture(0, std::make_shared<asset::Texture>((paimon_path / "Texture/脸.jpg").c_str(), false, 5));
  */
  CHECK_ERROR();

  //  ::scene::SerializeObject::SerializeScene(PublicSingleton<ConfigManage>::GetInstance().content_path /
  //  scene->m_title, *scene);
  //   ::scene::SerializeObject::DeserializeScene(PublicSingleton<ConfigManage>::GetInstance().content_path /
  //   scene->m_title, *scene);

  // 展示设置 800 600
  main_fbo = std::make_shared<asset::FBO>(1024, 576);
  main_fbo->set_color_texture();
  main_fbo->set_depth_texture();

  // scene->SubmitRender(quad.id);
  // scene->SubmitRender(cube.id);
  // scene->SubmitRender(sphere.id);
  // scene->SubmitRender(sphere_pbr.id);
  // scene->SubmitRender(paimon.id);
  scene->SubmitRender(aili.id);
  // 最后渲染
  // scene->SubmitRender(skybox.id);
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
  static bool demo_window_open = false;
  static bool new_scene_popup_open = false;
  static bool open_scene_popup_open = false;
  static bool save_scene_dir_popup_open = false;
  static bool save_scene_name_popup_open = false;
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
      if (ImGui::MenuItem("New", "Ctrl+N")) new_scene_popup_open = true;
      if (ImGui::MenuItem("Open...", "Ctrl+O")) open_scene_popup_open = true;
      if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
        save_scene_name_popup_open = true;
        save_scene_dir_popup_open = true;
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Help")) {
      if (ImGui::MenuItem("ImGui Demo Window")) {
        demo_window_open = true;
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  static bool hirerchy_panel_open = true;
  m_hierarchy_panel->OnImGuiRender(&hirerchy_panel_open, m_editor_camera);

  m_content_brower_panel->OnImGuiRender();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::SetNextWindowSize(ImVec2{static_cast<float>(main_fbo->Width()), static_cast<float>(main_fbo->Height())});
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

  if (demo_window_open) {
    ImGui::ShowDemoWindow(&demo_window_open);
  }

  if (new_scene_popup_open) {
    NewScene(new_scene_popup_open);
  }

  if (open_scene_popup_open) {
    OpenScene(open_scene_popup_open);
  }

  if (save_scene_dir_popup_open || save_scene_name_popup_open) {
    SaveScene(save_scene_dir_popup_open, save_scene_name_popup_open);
  }

  ImGui::End();

  ImGuiWrapper::End();
}

void EditorLayer::NewScene(bool& open) {
  ImGui::OpenPopup("New Scene");
  if (ImGui::BeginPopupModal("New Scene", &open)) {
    ImGui::Text("New Scene Name");
    std::array<char, 255> scene_name;
    scene_name.fill(0);
    ImGui::InputText("##scene name", scene_name.data(), sizeof(scene_name));

    if (ImGui::Button("confirm")) {
      m_cur_scene_name = std::string(scene_name.data());
      m_new_scene = std::make_shared<::scene::Scene>(m_cur_scene_name);
      m_cur_scene = std::move(m_new_scene);
      Reset();
      if (m_new_scene == nullptr) {
        CORE_DEBUG("old scene clean...");
      }

      open = false;
      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();
    if (ImGui::Button("Close")) {
      ImGui::CloseCurrentPopup();
      open = false;
    }
    ImGui::EndPopup();
  }
}

void EditorLayer::OpenScene(bool& open) {
  auto file_name_path = ImGuiWrapper::DrawFileBrower("Choose Scene", open, ".sa");
  if (file_name_path) {
    m_new_scene = std::make_shared<::scene::Scene>();
    ::scene::SerializeObject::DeserializeScene(*file_name_path, *m_new_scene);
    m_cur_scene = std::move(m_new_scene);
    m_hierarchy_panel->ResetScene(m_cur_scene);
    m_save_file_name_path = file_name_path;
    if (m_new_scene == nullptr) {
      CORE_DEBUG("old scene clean...");
    }
  }
}

void EditorLayer::SaveScene(bool& file_open, bool& name_open) {
  if (!m_save_file_name_path) {
    static std::optional<std::filesystem::path> save_scene_dir;
    if (file_open) {
      save_scene_dir = ImGuiWrapper::DrawDirBrower(file_open);
    }

    if (save_scene_dir) {
      ImGui::OpenPopup("Save Scene");
      if (ImGui::BeginPopupModal("Save Scene", &name_open)) {
        ImGui::Text("Save Scene Name");
        static std::string scene_name_string = "Unnamed";
        char scene_name[256];
        memset(scene_name, 0, sizeof name_open);
        strcpy(scene_name, scene_name_string.c_str());
        if (ImGui::InputText("##save name", scene_name, sizeof(scene_name))) {
          scene_name_string = std::string(scene_name);
        }

        if (ImGui::Button("confirm")) {
          CORE_DEBUG("scene name : {}", scene_name_string);
          std::string extention = ".sa";
          m_save_file_name_path = *save_scene_dir / (scene_name_string + extention);
          CORE_DEBUG("save name : {}", m_save_file_name_path->string());

          name_open = false;
          ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Close")) {
          name_open = false;
          ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
      }
    }
  } else {
    ::scene::SerializeObject::SerializeScene(*m_save_file_name_path, *m_cur_scene);

    file_open = false;
    name_open = false;
    CORE_INFO("Save scene succuss... (path = {})", m_save_file_name_path->string());
  }
}

void EditorLayer::TriggerViewPort() {
  // 未解除不能隐藏鼠标问题 初步判断和平台有问题
  ::core::Window::layer = ::core::Layer::ImGui;
  auto mouse_pos = ImGui::GetMousePos();
  bool is_pressed = ImGui::IsMouseDown(1);
  static bool is_first_pressed = true;
  static bool is_first_release = false;
  if (mouse_pos.x >= bound_viewport[0].x && mouse_pos.x <= bound_viewport[1].x && mouse_pos.y >= bound_viewport[0].y &&
      mouse_pos.y <= bound_viewport[1].y && is_pressed) {
    ::core::Window::layer = ::core::Layer::OnViewPort;

    if (is_first_pressed) {
      // 鼠标刚点击第一帧
      ::core::Window::bound_viewport_x = mouse_pos.x;
      ::core::Window::bound_viewport_y = mouse_pos.y;
      is_first_pressed = false;
    }
    is_first_release = true;
  } else if (is_first_release) {
    // 鼠标刚释放第一帧
    is_first_release = false;
    is_first_pressed = true;

    ::core::Input::clear();
  }
}

void EditorLayer::Reset() {
  m_hierarchy_panel->ResetScene(m_cur_scene);
  m_save_file_name_path.reset();
}

void EditorLayer::OnEvent() {}

void EditorLayer::CreateScene() {}

}  // namespace editor
}  // namespace saber
