#include <ImGuiFileDialog.h>
#include <core/Log.h>
#include <editor/ContentBrowerPanel.h>
#include <imGui/ImGuiWrapper.h>
#include <library/TextureLibrary.h>
#include <manage/ConfigManage.h>
#include <manage/AssetManage.h>

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
    ImGui::Text("This is content");

    if (m_selected_directory) {
      DrawFileBrower(*m_selected_directory);
      DrawContent();
    }
  }
  ImGui::EndChild();
  // ImGui::Columns(1);
  ImGui::End();
}

void ContentBrowerPanel::DrawTree() { DrawTreeRecursive(PublicSingleton<ConfigManage>::GetInstance().root_path); }

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

  // ImGui::SameLine();
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
      ImGui::PopStyleColor();
      ImGui::TextWrapped(path.path().filename().string().c_str(), "%s");
      ImGui::EndGroup();
      ImGui::NextColumn();
    }
  }
  ImGui::PopFont();
}

void ContentBrowerPanel::DrawFileBrower(const std::filesystem::path& to) {
  // open Dialog Simple

  ImTextureID texture_id =
      (ImTextureID)(intptr_t)PublicSingleton<Library<::asset::Texture>>::GetInstance().GetAddIcon()->get_id();
  if (ImGui::ImageButton(texture_id, {32, 32}, {0, 1}, {1, 0}))
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png,.jpg", ".");

  // display
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    // action if OK
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
      std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
      std::cout << "file pathname :" << filePathName << std::endl;
      std::cout << "file path" << filePath << std::endl;

      std::filesystem::path file_path_name = ImGuiFileDialog::Instance()->GetFilePathName();


      PublicSingleton<AssetManage>::GetInstance().Import(file_path_name, *m_selected_directory);

      // action
    }

    // close
    ImGuiFileDialog::Instance()->Close();
  }
}

}  // namespace saber
