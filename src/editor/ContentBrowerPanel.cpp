#include <editor/ContentBrowerPanel.h>
#include <imGui/ImGuiWrapper.h>
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
    ImGui::Text("This is content");
  }
  ImGui::EndChild();
  // ImGui::Columns(1);
  ImGui::End();
}

void ContentBrowerPanel::DrawTree() {
  DrawTreeRecursive(PublicSingleton<ConfigManage>::GetInstance().root_path);

}

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
  ImGui::Text(current_path.filename().string().c_str() ,"%s");

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

}  // namespace saber
