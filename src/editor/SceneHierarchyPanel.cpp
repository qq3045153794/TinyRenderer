#include <editor/SceneHierarchyPanel.h>
#include <imGui/ImGuiWrapper.h>
#include <library/TextureLibrary.h>
#include <library/ShaderLibrary.h>
#include <scene/Scene.h>
#include <component/Tag.hpp>
#include <cstring>

namespace saber {

using Scene = ::scene::Scene;
using Entity = ::scene::Entity;

SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<::scene::Scene> scene) : m_scene{scene}, m_select_entity{} {}

void SceneHierarchyPanel::OnImGuiRender(bool* hierarchy_open) {
  // ImGui::SetNextWindowPos(ImVec2{1024, 30});
  // ImGui::SetNextWindowSize(ImVec2{576, 288});
  ImGui::Begin("Entity Hierarchy");
  m_scene->registry.each([&](auto& entity_id) {
    Entity e{"temp", entity_id, &m_scene->registry};
    draw_entity_node(e);
  });

  // 右键点击弹出窗口
  if(ImGui::BeginPopupContextWindow()) {
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
        e.AddComponent<::component::Material>(default_shader);
        e.GetComponent<::component::Material>().set_texture(0, default_texture);
        m_scene->SubmitRender(e.id);
      }

      if (ImGui::MenuItem("Create Sphere")) {
        auto e = m_scene->create_entity("Sphere");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        e.AddComponent<::component::Mesh>(::component::Mesh::SPHERE);
        e.AddComponent<::component::Material>(default_shader);
        e.GetComponent<::component::Material>().set_texture(0, default_texture);
        m_scene->SubmitRender(e.id);
      }

      if (ImGui::MenuItem("Create Quad")) {
        auto e = m_scene->create_entity("Quad");
        auto default_shader = PublicSingleton<Library<Shader>>::GetInstance().GetDefaultShader();
        auto default_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture();
        e.AddComponent<::component::Mesh>(::component::Mesh::QUAD);
        e.AddComponent<::component::Material>(default_shader);
        e.GetComponent<::component::Material>().set_texture(0, default_texture);
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
    draw_components(m_select_entity);
  }
  ImGui::End();
}

static void draw_vec3_control(const std::string& label, glm::vec3& values, float reset_value,  float v_speed, float v_min, float v_max) {
  ImGui::Text(label.c_str(), "%s");
  ImGui::BeginTable("table_padding", 3, ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX);
  ImGui::TableNextRow();
  ImGui::TableSetColumnIndex(0);

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
  float lineHeight = 30.F;
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
  T& com = entity.GetComponent<T>();
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4.0f, 4.0f});
  ImGui::Separator();
  ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                       ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap |
                                       ImGuiTreeNodeFlags_FramePadding;
  bool open = ImGui::TreeNodeEx((void*)(void*)(uint64_t)(uint32_t)entity.id, tree_node_flags, name.c_str(), "%s");
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
  ImGui::SameLine();
  draw_component<component::Transform>("Transform", entity, [](auto& component) {
    auto temp_position = component.get_position();
    draw_vec3_control("Position", temp_position, 0.F , 0.1F, 0.0F, 0.0F);
    component.set_position(temp_position);

    auto temp_rotation = component.get_eular();
    draw_vec3_control("Rotation", temp_rotation, 0.F, 0.1F, 0.0F, 0.0F);
    component.set_ealar_YZX(temp_rotation);

    auto temp_scale = component.get_scale();
    draw_vec3_control("Scale", temp_scale, 1.0F, 0.1F, 0.0F, 0.0F);
    component.set_scale(temp_scale);
  });
}

void SceneHierarchyPanel::draw_entity_node(::scene::Entity& entity) {
  std::string local_name = entity.GetComponent<component::Tag>().m_name;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
  bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.id, flags, "");
  if (ImGui::IsItemClicked()) {
    m_select_entity = entity;
  }
  float lineHeight = 20.F;
  ImGui::SameLine();
  ImGui::Image((void*)(intptr_t)PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDefaultTexture()->get_id(),
               ImVec2{lineHeight - 5.0f, lineHeight - 5.0f}, {0, 1}, {1, 0});
  ImGui::SameLine();
  ImGui::Text(local_name.c_str(), "%s");

  if (opened) {
    ImGui::TreePop();
  }
}

}  // namespace saber
