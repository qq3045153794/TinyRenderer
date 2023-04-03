#include <editor/SceneHierarchyPanel.h>
#include <imGui/ImGuiWrapper.h>
#include <library/ShaderLibrary.h>
#include <library/TextureLibrary.h>
#include <manage/AssetManage.h>
#include <scene/Scene.h>

#include <component/Tag.hpp>
#include <cstring>

namespace saber {

using Scene = ::scene::Scene;
using Entity = ::scene::Entity;

SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<::scene::Scene> scene) : m_scene{scene}, m_select_entity{} {}

void SceneHierarchyPanel::ResetScene(std::shared_ptr<::scene::Scene> scene) {
  m_scene = scene;
  m_select_entity = ::scene::Entity();
}

void SceneHierarchyPanel::OnImGuiRender(bool* hierarchy_open) {
  // ImGui::SetNextWindowPos(ImVec2{1024, 30});
  // ImGui::SetNextWindowSize(ImVec2{576, 288});
  ImGui::Begin("Entity Hierarchy");
  m_scene->registry.each([&](auto& entity_id) {
    Entity e{"temp", entity_id, &m_scene->registry};
    draw_entity_node(e);
  });

  // 右键点击弹出窗口
  if (ImGui::BeginPopupContextWindow()) {
    if (ImGui::BeginMenu("Create Render Entity")) {
      if (ImGui::MenuItem("Create Empty")) {
        m_scene->create_entity("Empty");
        // TODO
      }

      if (ImGui::MenuItem("Create Cube")) {
        auto e = m_scene->create_entity("Cube");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        e.AddComponent<::component::Mesh>(::component::Mesh::CUBE);
        e.AddComponent<::component::Material>(::component::Material::ShadingModel::DEFAULT);
        e.GetComponent<::component::Material>().set_texture(0, default_texture);
        m_scene->SubmitRender(e.id);
      }

      if (ImGui::MenuItem("Create Sphere")) {
        auto e = m_scene->create_entity("Sphere");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        e.AddComponent<::component::Mesh>(::component::Mesh::SPHERE);
        e.AddComponent<::component::Material>(::component::Material::ShadingModel::DEFAULT);
        e.GetComponent<::component::Material>().set_texture(0, default_texture);
        m_scene->SubmitRender(e.id);
      }

      if (ImGui::MenuItem("Create Quad")) {
        auto e = m_scene->create_entity("Quad");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        e.AddComponent<::component::Mesh>(::component::Mesh::QUAD);
        e.AddComponent<::component::Material>(::component::Material::ShadingModel::DEFAULT);
        e.GetComponent<::component::Material>().set_texture(0, default_texture);
        m_scene->SubmitRender(e.id);
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Create PBR Entity")) {
      if (ImGui::MenuItem("Create PBR Empty")) {
        m_scene->create_entity("Empty");
        // TODO
      }

      if (ImGui::MenuItem("Create Cube##PBR")) {
        auto e = m_scene->create_entity("CubePBR");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        auto brdf_lut_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("BRDF_LUT");
        auto irradian_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("irradian");
        auto prefiltermap = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("prefiltermap");
        e.AddComponent<::component::Mesh>(::component::Mesh::CUBE);
        e.AddComponent<::component::Material>(::component::Material::ShadingModel::PBR);
        auto& mat = e.GetComponent<::component::Material>();
        mat.bind_texture(::component::Material::pbr_t::irradiance_map, irradian_texture);
        mat.bind_texture(::component::Material::pbr_t::prefilter_map, prefiltermap);
        mat.bind_texture(::component::Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
        m_scene->SubmitRender(e.id);
      }

      if (ImGui::MenuItem("Create Sphere##PBR")) {
        auto e = m_scene->create_entity("SpherePBR");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        auto brdf_lut_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("BRDF_LUT");
        auto irradian_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("irradian");
        auto prefiltermap = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("prefiltermap");
        e.AddComponent<::component::Mesh>(::component::Mesh::SPHERE);
        e.AddComponent<::component::Material>(::component::Material::ShadingModel::PBR);
        auto& mat = e.GetComponent<::component::Material>();
        mat.bind_texture(::component::Material::pbr_t::irradiance_map, irradian_texture);
        mat.bind_texture(::component::Material::pbr_t::prefilter_map, prefiltermap);
        mat.bind_texture(::component::Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
        m_scene->SubmitRender(e.id);
      }

      if (ImGui::MenuItem("Create Quad##PBR")) {
        auto e = m_scene->create_entity("QuadPBR");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        auto brdf_lut_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("BRDF_LUT");
        auto irradian_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("irradian");
        auto prefiltermap = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("prefiltermap");
        e.AddComponent<::component::Mesh>(::component::Mesh::QUAD);
        e.AddComponent<::component::Material>(::component::Material::ShadingModel::PBR);
        e.GetComponent<::component::Material>().set_texture(0, default_texture);
        auto& mat = e.GetComponent<::component::Material>();
        mat.bind_texture(::component::Material::pbr_t::irradiance_map, irradian_texture);
        mat.bind_texture(::component::Material::pbr_t::prefilter_map, prefiltermap);
        mat.bind_texture(::component::Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
        m_scene->SubmitRender(e.id);
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Create Camera Entity")) {
      if (ImGui::MenuItem("Create Camera")) {
        // TODO
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Create Light")) {
      if (ImGui::MenuItem("Create Dirct Light")) {
        // TODO
      }

      if (ImGui::MenuItem("Create Point Light")) {
        // TODO
      }
      ImGui::EndMenu();
    }

    ImGui::EndPopup();
  }
  ImGui::End();

  // ImGui::SetNextWindowPos(ImVec2{1024, 318});
  // ImGui::SetNextWindowSize(ImVec2{576, 288});
  ImGui::Begin("Entity Property");
  if (m_select_entity.id != entt::null) {
    ImGui::PushFont(ImGuiWrapper::config_font);
    draw_components(m_select_entity);
    ImGui::PopFont();
  }
  ImGui::End();
}

static void draw_vec3_control(const std::string& label, glm::vec3& values, float reset_value, float v_speed,
                              float v_min, float v_max) {
  ImGui::Text(label.c_str(), "%s");
  ImGui::BeginTable("table_padding", 3, ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX);
  ImGui::TableNextRow();
  ImGui::TableSetColumnIndex(0);

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
  float lineHeight = 25.F;
  ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});

  // 同一个window下的botton id必须不同，否则会冲突导致按键没反应
  // 可用##str 将str作为区分id部分，且str不会显示在ui中
  // 参考：https://github.com/ocornut/imgui/wiki#about-the-imgui-paradigm
  std::string x_botton_id = "X##" + label;
  if (ImGui::Button(x_botton_id.c_str(), buttonSize)) {
    CORE_DEBUG("Click X botton (label = {})", label);
    values.x = reset_value;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat(("##X" + label).c_str(), &values.x, v_speed, v_min, v_max, "%.2f");
  ImGui::SameLine();

  ImGui::TableSetColumnIndex(1);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
  std::string y_botton_id = "Y##" + label;
  if (ImGui::Button(y_botton_id.c_str(), buttonSize)) {
    CORE_DEBUG("Click Y botton (label = {})", label);
    values.y = reset_value;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat(("##Y" + label).c_str(), &values.y, v_speed, v_min, v_max, "%.2f");
  ImGui::SameLine();

  ImGui::TableSetColumnIndex(2);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
  std::string z_botton_id = ("Z##" + label).c_str() + label;
  if (ImGui::Button(z_botton_id.c_str(), buttonSize)) {
    CORE_DEBUG("Click Z botton (label = {})", label);
    values.z = reset_value;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  ImGui::DragFloat("##Z", &values.z, v_speed, v_min, v_max, "%.2f");
  ImGui::SameLine();

  ImGui::PopStyleVar();
  ImGui::EndTable();
}

template <typename T, typename FUNC>
static void draw_component(const std::string& name, scene::Entity entity, FUNC uiFunction) {
  if (!entity.Contains<T>()) {
    return;
  }
  T& com = entity.GetComponent<T>();
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4.0f, 4.0f});
  ImGui::Separator();
  ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                       ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
                                       ImGuiTreeNodeFlags_FramePadding;
  // bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.id, tree_node_flags, name.c_str(), "%s");
  bool open = ImGui::TreeNodeEx(name.c_str(), tree_node_flags, name.c_str(), "%s");
  ImGui::PopStyleVar();
  if (open) {
    uiFunction(com);

    ImGui::TreePop();
  }
}

void SceneHierarchyPanel::draw_components(Entity& entity) {
  if (std::string& name = entity.GetComponent<component::Tag>().m_name; true) {
    char buffer[256];
    memset(buffer, 0, sizeof buffer);
    strcpy(buffer, name.c_str());
    if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
      name = std::string(buffer);
    }
  }
  // ImGui::SameLine();
  draw_component<component::Transform>("Transform", entity, [](auto& component) {
    auto temp_position = component.get_position();
    draw_vec3_control("Position", temp_position, 0.F, 0.1F, 0.0F, 0.0F);
    component.set_position(temp_position);

    auto temp_rotation = component.get_eular();
    draw_vec3_control("Rotation", temp_rotation, 0.F, 0.1F, 0.0F, 0.0F);
    component.set_ealar_YZX(temp_rotation);

    auto temp_scale = component.get_scale();
    draw_vec3_control("Scale", temp_scale, 1.0F, 0.1F, 0.0F, 0.0F);
    component.set_scale(temp_scale);
  });

  // ImGui::SameLine();

  draw_component<::component::Material>("Material", entity, [this](::component::Material& component) {
    using Material = ::component::Material;
    auto& textures_cache = PublicSingleton<AssetManage>::GetInstance().textures_cache;
    if (component.m_shading_model == Material::ShadingModel::PBR) {
      if (ImGui::Text("Albedo"); true) {
        ImGui::BeginTable("##Albedo", 2);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        float f_albedo[4];
        auto ubo_var = component.get_uniform(Material::pbr_u::albedo);
        auto albedo = std::get<::component::UboData<glm::vec4>>(ubo_var).m_data;
        auto sample_albedo_var = component.get_uniform("sample_albedo");
        bool sample_albedo = std::get<::component::UboData<bool>>(sample_albedo_var).m_data;
        for (std::size_t i = 0; i < 4; i++) f_albedo[i] = albedo[i];
        if (sample_albedo) ImGui::BeginDisabled();
        ImGui::ColorEdit4("##albedo", f_albedo);
        if (sample_albedo) ImGui::EndDisabled();
        for (std::size_t i = 0; i < 4; i++) albedo[i] = f_albedo[i];
        component.bind_uniform(Material::pbr_u::albedo, albedo);

        ImGui::TableSetColumnIndex(1);
        std::vector<std::filesystem::path> albedo_items;
        static const char* current_item = nullptr;
        static Entity last_entity;
        if (last_entity.id != m_select_entity.id) current_item = nullptr;
        last_entity = m_select_entity;
        albedo_items.push_back("None");
        if (sample_albedo) {
          auto albedo = component.get_texture(Material::pbr_t::albedo);
          if(current_item == nullptr) current_item = albedo->m_image_path->c_str();
          CORE_ASERT(textures_cache.count(*albedo->m_image_path) > 0, "No import the Texture (path = {})",
                     albedo->m_image_path->string());
          // albedo_items.push_back(*albedo->m_image_path);
        }

        if(current_item == nullptr) CORE_DEBUG("current_item is NULL");
        if (current_item == nullptr) current_item = "None";

        for (const auto& [path, texture] : textures_cache) {
          albedo_items.push_back(path);
        }
        CORE_DEBUG("current item : {}", current_item);
        if (ImGui::BeginCombo("##Albedo", current_item)) {
          for (const auto& item : albedo_items) {
            bool is_selected = (strcmp(current_item, item.c_str()) == 0);
            if (ImGui::Selectable(item.c_str(), is_selected)) {
              current_item = item.c_str();
              CORE_DEBUG("click {}", item.string());
            }

            if (is_selected && item == "None") {
              component.set_uniform(100U, false);

              ImGui::SetItemDefaultFocus();
            } else if (is_selected && item != "None") {
              CORE_ASERT(textures_cache.count(item) > 0, "No find texture in textures cache (key = {})", item.string());
              auto albedo = textures_cache[item];
              component.bind_texture(Material::pbr_t::albedo, albedo);

              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }

        ImGui::EndTable();
      }

      if (ImGui::Text("Roughness"); true) {
        ImGui::BeginTable("##Roughness", 1);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        auto ubo_var = component.get_uniform(Material::pbr_u::roughness);
        float reghness = std::get<::component::UboData<float>>(ubo_var).m_data;
        auto sample_reghness_var = component.get_uniform("sample_roughness");
        bool sample_reghness = std::get<::component::UboData<bool>>(sample_reghness_var).m_data;
        if (sample_reghness) ImGui::BeginDisabled();
        ImGui::DragFloat("##Roughness", &reghness, 0.01F, 0.F, 1.F, "%.2f");
        if (sample_reghness) ImGui::EndDisabled();
        component.bind_uniform(Material::pbr_u::roughness, reghness);
        ImGui::EndTable();
      }

      if (ImGui::Text("Metalness"); true) {
        ImGui::BeginTable("##Metalness", 1);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        auto ubo_var = component.get_uniform(Material::pbr_u::metalness);
        float metalness = std::get<::component::UboData<float>>(ubo_var).m_data;
        auto sample_metalness_var = component.get_uniform("sample_metalness");
        bool sample_metalness = std::get<::component::UboData<bool>>(sample_metalness_var).m_data;
        if (sample_metalness) ImGui::BeginDisabled();
        ImGui::DragFloat("##Metalness", &metalness, 0.01F, 0.F, 1.F, "%.2f");
        if (sample_metalness) ImGui::EndDisabled();
        component.bind_uniform(Material::pbr_u::metalness, metalness);
        ImGui::EndTable();
      }

      if (ImGui::Text("Specular"); true) {
        ImGui::BeginTable("##Specular", 1);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        auto ubo_var = component.get_uniform(Material::pbr_u::specular);
        float specular = std::get<::component::UboData<float>>(ubo_var).m_data;
        ImGui::DragFloat("##Specular", &specular, 0.01F, 0.F, 1.F, "%.2f");
        component.bind_uniform(Material::pbr_u::specular, specular);
        ImGui::EndTable();
      }

      if (ImGui::Text("AO"); true) {
        ImGui::BeginTable("##Ao", 1);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        auto ubo_var = component.get_uniform(Material::pbr_u::ao);
        float ao = std::get<::component::UboData<float>>(ubo_var).m_data;
        auto sample_ao_var = component.get_uniform("sample_ao");
        bool sample_ao = std::get<::component::UboData<bool>>(sample_ao_var).m_data;
        if (sample_ao) ImGui::BeginDisabled();
        ImGui::DragFloat("##Ao", &ao, 0.01F, 0.F, 1.F, "%.2f");
        if (sample_ao) ImGui::EndDisabled();
        component.bind_uniform(Material::pbr_u::ao, ao);
        ImGui::EndTable();
      }
    }
  });
}

void SceneHierarchyPanel::draw_entity_node(::scene::Entity& entity) {
  std::string local_name = entity.GetComponent<component::Tag>().m_name;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
  if (m_select_entity.id == entity.id) {
    flags |= ImGuiTreeNodeFlags_Selected;
  }
  bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.id, flags, "");
  if (ImGui::IsItemClicked()) {
    m_select_entity = entity;
  }
  float lineHeight = 20.F;
  ImGui::SameLine();
  ImGui::Image((void*)(intptr_t)PublicSingleton<Library<::asset::Texture>>::GetInstance().GetRabbitIcon()->get_id(),
               ImVec2{lineHeight, lineHeight}, {0, 1}, {1, 0});
  ImGui::SameLine();
  ImGui::Text(local_name.c_str(), "%s");

  if (opened) {
    ImGui::TreePop();
  }
}

}  // namespace saber
