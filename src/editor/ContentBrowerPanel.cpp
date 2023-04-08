#include <ImGuiFileDialog.h>
#include <core/Log.h>
#include <editor/ContentBrowerPanel.h>
#include <imGui/ImGuiWrapper.h>
#include <library/TextureLibrary.h>
#include <manage/AssetManage.h>
#include <manage/ConfigManage.h>

namespace saber {

ContentBrowerPanel::ContentBrowerPanel() : m_current_directory() {}

void ContentBrowerPanel::OnImGuiRender() {
  ImGui::Begin("Content Brower");

  ImGui::Columns(2);

  // 让其可以拖动
  static bool init = true;
  if (init) {
    ImGui::SetColumnWidth(0, 240.F);
    init = false;
  }

  if (ImGui::BeginChild("Tree")) {
    DrawTree();
  }
  ImGui::EndChild();

  ImGui::NextColumn();

  if (ImGui::BeginChild("Content")) {
    if (m_selected_directory) {
      DrawSetting();
      ImGui::Separator();
      DrawContent();
    }
  }
  ImGui::EndChild();
  ImGui::End();
}

void ContentBrowerPanel::DrawTree() { DrawTreeRecursive(PublicSingleton<ConfigManage>::GetInstance().content_path); }

void ContentBrowerPanel::DrawTreeRecursive(const std::filesystem::path& current_path) {
  ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                  ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
  if (m_selected_directory && *m_selected_directory == current_path) {
    node_flags |= ImGuiTreeNodeFlags_Selected;
  }
  std::string label = "##" + current_path.filename().string();
  bool node_open = ImGui::TreeNodeEx(label.c_str(), node_flags);
  if (ImGui::IsItemClicked()) {
    m_selected_directory = current_path;
    std::cout << "selected : " << m_selected_directory->filename().string() << std::endl;
  }

  ImGui::SameLine();
  ImGui::Text(current_path.filename().string().c_str(), "%s");

  if (node_open) {
    for (auto& path : std::filesystem::directory_iterator(current_path)) {
      if (!std::filesystem::is_directory(path)) {
        continue;
      }
      DrawTreeRecursive(path);
    }
    ImGui::TreePop();
  }
}

void ContentBrowerPanel::DrawContent() {
  m_current_directory = *m_selected_directory;

  static float padding = 32.0F;
  static float content_size = 32.0F;
  float cell_size = content_size + padding;

  float panelWidth = ImGui::GetContentRegionAvail().x;
  int columnCount = (int)(panelWidth / cell_size);
  if (columnCount < 1) columnCount = 1;

  ImGui::Columns(columnCount, 0, false);
  std::vector<std::filesystem::path> contents;

  ImGui::PushFont(ImGuiWrapper::config_font);
  for (auto& path : std::filesystem::directory_iterator(m_current_directory)) {
    bool is_register = PublicSingleton<AssetManage>::GetInstance().CheckResExit(path);
    if (!is_directory(path) && is_register) {
      ImGui::BeginGroup();
      ImTextureID texture_id =
          (void*)(intptr_t)PublicSingleton<Library<::asset::Texture>>::GetInstance().GetTxtFileIcon()->get_id();
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
      ImGui::ImageButton(texture_id, {content_size, content_size}, {0, 1}, {1, 0});

      if (ImGui::BeginDragDropSource()) {
        auto filename_path = std::filesystem::path(path);
        const char* drag_data = filename_path.c_str();
        std::size_t drag_data_size = sizeof(char) * strlen(drag_data);
        ImGui::SetDragDropPayload("CONTENT_BROWER_ITEM", drag_data, drag_data_size, ImGuiCond_Once);
        ImGui::Text("drag : %s", drag_data);
        ImGui::EndDragDropSource();
      }

      ImGui::PopStyleColor();
      ImGui::TextWrapped(path.path().filename().string().c_str(), "%s");
      ImGui::EndGroup();
      ImGui::NextColumn();
    }
  }
  ImGui::PopFont();
}

void ContentBrowerPanel::DrawSetting() {
  // Import Button
  ImTextureID texture_id =
      (ImTextureID)(intptr_t)PublicSingleton<Library<::asset::Texture>>::GetInstance().GetAddIcon()->get_id();
  static bool file_brower_open = false;
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.F, 0.F, 0.F, 0.F));
  if (ImGui::ImageButton(texture_id, {24, 24}, {0, 1}, {1, 0})) {
    file_brower_open = true;
  }
  if (file_brower_open) {
    auto file_path_name = ImGuiWrapper::DrawFileBrower("Choose image", file_brower_open, ".png,.jpg,.jpeg");
    if (file_path_name) {
      PublicSingleton<AssetManage>::GetInstance().Import(*file_path_name, *m_selected_directory);
    }
  }
  ImGui::PopStyleColor();

  ImGui::SameLine();
  // Delete Botton
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.F, 0.F, 0.F, 1.F));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0, 0.0, 0.0, 1.0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0, 0.0, 0.0, 1.0));
  ImTextureID texture_delete_id =
      (ImTextureID)(intptr_t)PublicSingleton<Library<::asset::Texture>>::GetInstance().GetDeleteIcon()->get_id();
  ImGui::ImageButton(texture_delete_id, {24, 24}, {0, 1}, {1, 0});
  if (ImGui::BeginDragDropTarget()) {

    if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWER_ITEM")) {
      const char* data = static_cast<const char*>(payload->Data);
      CORE_DEBUG("Get data : {}", data);
      PublicSingleton<AssetManage>::GetInstance().Remove(data);
    }
    ImGui::EndDragDropTarget();
  }
  ImGui::PopStyleColor(3);
}

void ContentBrowerPanel::DrawSave() {
  if (ImGui::Button("Save Resource")) {
    PublicSingleton<AssetManage>::GetInstance().SaveConfig();
  }
}

}  // namespace saber
