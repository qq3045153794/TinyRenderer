#include <core/Window.h>
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

void SceneHierarchyPanel::OnImGuiRender(bool* hierarchy_open, scene::Entity& editor_camera) {
  ImGui::Begin("Entity Hierarchy");
  m_scene->registry.each([&](auto& entity_id) {
    Entity e{"temp", entity_id, &m_scene->registry};
    draw_entity_node(e);
  });

  // editor camera生命周期不随着Scene
  // 需要单独绘制editor camera
  draw_entity_node(editor_camera);
  ImTextureID delete_icon =
      (ImTextureID)(intptr_t)PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDeleteIcon()->get_id();
  if (m_select_entity.id != entt::null) {
    ImVec2 icon_size = {24, 24};
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0, 0.0, 0.0, 0.0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0, 0.0, 0.0, 1.0));
    if (ImGui::ImageButton("Delete", delete_icon, icon_size, {0, 1}, {1, 0})) {
      m_scene->delete_entity(m_select_entity.id);
      m_select_entity = Entity();
    }
    ImGui::PopStyleColor(2);
  }

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

  draw_component<component::Model>("Model", entity, [](component::Model& component) {
    auto& textures_cache = PublicSingleton<AssetManage>::GetInstance().textures_cache;
    std::vector<std::string> items;
    items.push_back("DEFAULT");
    auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
    for (const auto& [path, texture] : textures_cache) {
      items.push_back(path);
    }
    ImGui::PushFont(ImGuiWrapper::u8_font);
    for (auto& [mat_name, uid] : component.materials_cache) {
      auto& mat = component.materials.at(uid);
      if (mat.m_shading_model == ::component::Material::ShadingModel::DEFAULT) {
        std::string current_item = mat.get_texture(0)->m_image_path->string();
        if (current_item == default_texture->m_image_path->string()) {
          current_item = "DEFAULT";
        }
        ImGui::Text("%s : ", mat_name.c_str());
        if (ImGui::BeginCombo(("##" + mat_name).c_str(), current_item.c_str())) {
          for (const auto& item : items) {
            bool is_selected = (current_item == item);
            if (ImGui::Selectable(item.c_str(), is_selected)) {
              if (item == "DEFAULT") {
                auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
                mat.set_texture(0, default_texture);
              } else {
                mat.set_texture(0, std::make_shared<::asset::Texture>(item.c_str(), false, 7U));
              }
            }

            if (is_selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
    }
    ImGui::PopFont();
  });

  draw_component<::component::Material>("Material", entity, [&entity](::component::Material& component) {
    using Material = ::component::Material;
    auto& textures_cache = PublicSingleton<AssetManage>::GetInstance().textures_cache;

    auto PbrTextureCombo = [&textures_cache](const std::string& combo_key, Material::pbr_t pbr, auto& current_item,
                                             auto& component, const Entity& entity, bool is_samle) -> void {
      std::vector<std::filesystem::path> items;
      items.push_back("None");
      if (is_samle) {
        if (!current_item) {
          auto texture = component.get_texture(pbr);
          current_item = texture->m_image_path->string();
          CORE_ASERT(textures_cache.count(*texture->m_image_path) > 0, "No import the Texture (path = {})",
                     texture->m_image_path->string());
        }
      }

      if (!current_item) {
        current_item = "None";
      }

      for (const auto& [path, texture] : textures_cache) {
        items.push_back(path);
      }

      ImGuiWrapper::DrawCombo(combo_key, items, current_item, [&component, &textures_cache, &pbr](const auto& item) {
        if (item == "None") {
          // 由于是顺序的 可以通过计算来映射
          component.set_uniform(95U + static_cast<uint32_t>(pbr), false);
        } else {
          auto texture = textures_cache[item];
          component.bind_texture(pbr, texture);
        }
      });
    };

    auto TextureCombo = [&textures_cache](const std::string& combo_key, auto& component, const Entity& entity) -> void {
      std::vector<std::filesystem::path> items;
      items.push_back("DEFAULT");
      auto texture = component.get_texture(0U);
      std::optional<std::string> current_item = texture->m_image_path->string();

      auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
      if (*current_item == default_texture->m_image_path->string()) current_item = "DEFAULT";

      for (const auto& [path, texture] : textures_cache) {
        items.push_back(path);
      }

      ImGuiWrapper::DrawCombo(combo_key, items, current_item, [&component, &textures_cache](const auto& item) {
        if (item == "DEFAULT") {
          // 由于是顺序的 可以通过计算来映射
          // component.set_uniform(95U + static_cast<uint32_t>(pbr), false);
          auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
          component.set_texture(0, default_texture);
        } else {
          auto texture = textures_cache[item];
          component.set_texture(0, texture);
        }
      });
    };

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
        static std::optional<std::string> current_item;
        static Entity last_entity;
        if (last_entity.id != entity.id) {
          current_item.reset();
        }
        last_entity = entity;
        PbrTextureCombo("##AlbedoCombo", Material::pbr_t::albedo, current_item, component, entity, sample_albedo);
        ImGui::EndTable();
      }

      if (ImGui::Text("Roughness"); true) {
        ImGui::BeginTable("##Roughness", 2);
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

        ImGui::TableSetColumnIndex(1);
        static std::optional<std::string> current_item;
        static Entity last_entity;
        if (last_entity.id != entity.id) {
          current_item.reset();
        }
        last_entity = entity;
        PbrTextureCombo("##RoughnessCombo", Material::pbr_t::roughness, current_item, component, entity,
                        sample_reghness);
        ImGui::EndTable();
      }

      if (ImGui::Text("Metalness"); true) {
        ImGui::BeginTable("##Metalness", 2);
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

        ImGui::TableSetColumnIndex(1);
        static std::optional<std::string> current_item;
        static Entity last_entity;
        if (last_entity.id != entity.id) {
          current_item.reset();
        }
        last_entity = entity;
        PbrTextureCombo("##MetalnessCombo", Material::pbr_t::metalness, current_item, component, entity,
                        sample_metalness);
        ImGui::EndTable();
      }

      if (ImGui::Text("Specular"); true) {
        ImGui::BeginTable("##Specular", 1);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        auto ubo_var = component.get_uniform(Material::pbr_u::specular);
        float specular = std::get<::component::UboData<float>>(ubo_var).m_data;
        ImGui::DragFloat("##SpecularCombo", &specular, 0.01F, 0.F, 1.F, "%.2f");
        component.bind_uniform(Material::pbr_u::specular, specular);
        ImGui::EndTable();
      }

      if (ImGui::Text("AO"); true) {
        ImGui::BeginTable("##Ao", 2);
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

        ImGui::TableSetColumnIndex(1);
        static std::optional<std::string> current_item;
        static Entity last_entity;
        if (last_entity.id != entity.id) {
          current_item.reset();
        }
        last_entity = entity;
        PbrTextureCombo("##AoCombo", Material::pbr_t::ao, current_item, component, entity, sample_ao);
        ImGui::EndTable();
      }

      if (ImGui::Text("Normal"); true) {
        ImGui::BeginTable("##NorTable", 1);
        ImGui::TableNextRow();
        auto sample_normal_var = component.get_uniform("sample_normal");
        bool sample_normal = std::get<::component::UboData<bool>>(sample_normal_var).m_data;

        ImGui::TableSetColumnIndex(0);
        static std::optional<std::string> current_item;
        static Entity last_entity;
        if (last_entity.id != entity.id) {
          current_item.reset();
        }
        last_entity = entity;
        PbrTextureCombo("##NormalCombo", Material::pbr_t::normal, current_item, component, entity, sample_normal);
        ImGui::EndTable();
      }
    }

    if (component.m_shading_model == Material::ShadingModel::DEFAULT) {
      ImGui::BeginTable("##Texture", 1);
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      // static std::optional<std::string> current_item;
      // static Entity last_entity;
      // if (last_entity.id != entity.id) {
      //   current_item.reset();
      // }
      // last_entity = entity;
      TextureCombo("##TexutreCombo", component, entity);
      ImGui::EndTable();
    }
  });

  draw_component<component::Mesh>("Mesh", entity, [&entity](component::Mesh& component) {
    ImGui::BeginTable("##NorTable", 1);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    std::array<std::string, 3U> items = {"QUAD", "CUBE", "SPHERE"};
    std::string current_item = component.get_primitive_strings();
    if (ImGui::BeginCombo("##MeshComb", current_item.c_str())) {
      for (const auto& item : items) {
        bool is_selected = (current_item == item);
        if (ImGui::Selectable(item.c_str(), is_selected)) {
          auto primitive = ::component::Mesh::cast_primitive(item);
          entity.SetComponent<::component::Mesh>(primitive);
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
    ImGui::EndTable();
  });

  draw_component<component::CameraFps>("CameraFps", entity, [&entity](component::CameraFps& component) {
    if (component.get_projection_string() == "PERSPECTIVE") {
      ImGui::Text("Aspect");
      ImGui::DragFloat("##AspectDrag", &component.Aspect(), 0.01F, 0.1F, 10.F, "%.2f");
      ImGui::Text("FOV");
      ImGui::DragFloat("##FovDrag", &component.Fov(), 1.F, 10.F, 150.F, "%.0f");
    } else if (component.get_projection_string() == "ORTHOGRAPHIC") {
      ImGui::Text("Left");
      ImGui::DragFloat("##LeftDrag", &component.Left(), 0.1F, -10.F, 10.F, "%.2f");
      ImGui::Text("Right");
      ImGui::DragFloat("##RightDrag", &component.Right(), 0.1F, -10.0F, 10.F, "%.2f");
      ImGui::Text("Bottom");
      ImGui::DragFloat("##BottomDrag", &component.Bottom(), 0.1F, -10.F, 10.F, "%.2f");
      ImGui::Text("Top");
      ImGui::DragFloat("##TopDrag", &component.Top(), 0.1F, -10.F, 10.F, "%.2f");
    }

    ImGui::Text("zNear");
    ImGui::DragFloat("##zNearDrag", &component.Znear(), 1.F, 0.1F, 100.F, "%.0f");
    ImGui::Text("FZFar");
    ImGui::DragFloat("##zFarFovDrag", &component.Zfar(), 1.F, 0.1F, 100.F, "%.0f");

    ImGui::Text("Projection");
    std::array<std::string, 2U> items = {"PERSPECTIVE", "ORTHOGRAPHIC"};
    std::string current_item = component.get_projection_string();
    if (ImGui::BeginCombo("##ProjectionComb", current_item.c_str())) {
      for (const auto& item : items) {
        bool is_selected = (current_item == item);
        if (ImGui::Selectable(item.c_str(), is_selected)) {
          auto& transform = entity.GetComponent<::component::Transform>();
          float camera_width = static_cast<float>(core::Window::m_width);
          float camera_height = static_cast<float>(core::Window::m_height);
          float default_znear = 0.1F;
          float default_zfar = 100.F;
          if (item == "PERSPECTIVE") {
            float default_fov = 60.F;
            entity.SetComponent<::component::CameraFps>(&transform, default_fov, camera_width / camera_height,
                                                        default_znear, default_zfar);
          } else if (item == "ORTHOGRAPHIC") {
            entity.SetComponent<::component::CameraFps>(&transform, -1.F, 1.F, -1.F, 1.F, default_znear, default_zfar);
          }
        }

        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndCombo();
    }
  });

  draw_component<component::PointLight>("Point Light", entity, [](component::PointLight& component) {
    ImGui::Text("Color");
    float f_color[3];
    for (int i = 0; i < 3; i++) {
      f_color[i] = component.m_color[i];
    }
    ImGui::ColorEdit3("##Point Color", f_color);
    for (int i = 0; i < 3; i++) {
      component.m_color[i] = f_color[i];
    }
    ImGui::Text("Intensity");
    ImGui::DragFloat("##IntensityDrag", &component.m_intensity, 0.02F, 0.F, 2.F, "%.2f");
    ImGui::Text("Linear");
    ImGui::DragFloat("##LinearDrag", &component.m_linear, 0.02F, 0.F, 2.F, "%.2f");
    ImGui::Text("Quadratic");
    ImGui::DragFloat("##QuadraticDrag", &component.m_quadratic, 0.02F, 0.F, 2.F, "%.2f");
  });

  draw_component<component::DirectionLight>("Direction Light", entity, [](component::DirectionLight& component) {
    ImGui::Text("Color");
    float f_color[3];
    for (int i = 0; i < 3; i++) {
      f_color[i] = component.m_color[i];
    }
    ImGui::ColorEdit3("##Direction Color", f_color);
    for (int i = 0; i < 3; i++) {
      component.m_color[i] = f_color[i];
    }
    ImGui::Text("Intensity");
    ImGui::DragFloat("##DirectionIntensityDrag", &component.m_intensity, 0.02F, 0.F, 2.F, "%.2f");
  });

  ImGui::Separator();
  ImGui::PushFont(ImGuiWrapper::main_font);
  ImVec2 botton_text_size = ImGui::CalcTextSize("Add component");
  float avail = ImGui::GetContentRegionAvail().x / 2 - botton_text_size.x / 2;
  ImGui::SetCursorPosX(avail);
  if (ImGui::Button("Add Component", {botton_text_size.x + 24, botton_text_size.y + 16})) {
    ImGui::OpenPopup("Add Conponent up");
  }

  if (ImGui::BeginPopup("Add Conponent up")) {
    if (ImGui::MenuItem("hello world")) {
    }

    if (!entity.Contains<::component::Material>()) {
      if (ImGui::MenuItem("Add PBR Material")) {
        entity.AddComponent<::component::Material>(::component::Material::ShadingModel::PBR);
      }
    }

    if (!entity.Contains<::component::Material>()) {
      if (ImGui::MenuItem("Add DEFAULT Material")) {
        auto& component = entity.AddComponent<::component::Material>(::component::Material::ShadingModel::DEFAULT);
        component.set_texture(0U, PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture());
      }
    }

    if (!entity.Contains<::component::Mesh>()) {
      if (ImGui::MenuItem("Add Mesh")) {
        entity.AddComponent<::component::Mesh>(::component::Mesh::primitive::SPHERE);
        m_scene->SubmitRender(entity.id);
      }
    }

    if (!entity.Contains<::component::DirectionLight>()) {
      if (ImGui::MenuItem("Add Direction Light")) {
        entity.AddComponent<::component::DirectionLight>(glm::vec3(1.F, 1.F, 1.F), 1.F);
      }
    }

    if (!entity.Contains<::component::PointLight>()) {
      if (ImGui::MenuItem("Add Point Light")) {
        auto& component = entity.AddComponent<::component::PointLight>(glm::vec3(1.F, 1.F, 1.F), 1.F);
        component.set_attenuation(0.F, 0.F);
      }
    }
    ImGui::EndPopup();
  }

  ImGui::PopFont();
}

void SceneHierarchyPanel::draw_entity_node(::scene::Entity& entity) {
  std::string local_name = entity.GetComponent<component::Tag>().m_name;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
  // 由于editor camera的entity_id 会和 scene里的entity冲突
  if (m_select_entity.id == entity.id && m_select_entity.name != "editor camera" && entity.name != "editor camera") {
    flags |= ImGuiTreeNodeFlags_Selected;
  } else if (entity.name == m_select_entity.name && m_select_entity.name == "editor camera") {
    flags |= ImGuiTreeNodeFlags_Selected;
  }
  void* uid = (void*)(uint64_t)(uint32_t)entity.id;
  bool opened;
  if (entity.name != "editor camera") {
    opened = ImGui::TreeNodeEx(uid, flags, "");
  } else {
    opened = ImGui::TreeNodeEx("##Editor Camera", flags);
  }

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
