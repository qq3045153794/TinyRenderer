#include <imGui/ImGuiWrapper.h>
#include <core/Window.h>

namespace saber {

static const char* glsl_version = "#version 330";

static ImFont* truetype_font;  // TrueType, Lato-Regular, 18pt (main font)

void ImGuiWrapper::Init() {
  using Window = ::core::Window;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGuiStyle& style = ImGui::GetStyle();

  // 设置字体
  std::string ttf_main = "../resource/fronts/Lato.ttf";
  float font_main_sz = 18.0f;
  ImFontConfig config_main;
  config_main.PixelSnapH = true;
  config_main.OversampleH = 4;
  config_main.OversampleV = 4;
  config_main.RasterizerMultiply = 1.2f;   // brighten up the font to make them more readable
  config_main.GlyphExtraSpacing.x = 0.0f;  // 字间距离
  truetype_font = io.Fonts->AddFontFromFileTTF(ttf_main.c_str(), font_main_sz, &config_main);

  // load default dark theme
  ImGui::StyleColorsDark();

  // setup custom styles
  style.WindowBorderSize = 0.0f;
  style.FrameBorderSize = 1.0f;
  style.PopupBorderSize = 1.0f;
  style.ChildBorderSize = 1.0f;
  style.TabBorderSize = 0.0f;
  style.ScrollbarSize = 18.0f;
  style.GrabMinSize = 10.0f;

  style.WindowPadding = ImVec2(8.0f, 8.0f);
  style.FramePadding = ImVec2(4.0f, 6.0f);
  style.ItemSpacing = ImVec2(10.0f, 10.0f);
  style.ItemInnerSpacing = ImVec2(10.0f, 10.0f);
  style.IndentSpacing = 16.0f;

  style.WindowRounding = 0.0f;
  style.ChildRounding = 0.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 0.0f;
  style.TabRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.ScrollbarRounding = 12.0f;

  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ColorButtonPosition = ImGuiDir_Right;

  style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
  style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
  style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

  style.AntiAliasedLines = true;
  style.AntiAliasedFill = true;
  style.AntiAliasedLinesUseTex = true;

  // setup custom colors
  auto& c = ImGui::GetStyle().Colors;

  c[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.85f);
  c[ImGuiCol_ChildBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.85f);
  c[ImGuiCol_PopupBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.85f);

  c[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);
  c[ImGuiCol_FrameBgHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.75f);
  c[ImGuiCol_FrameBgActive] = ImVec4(0.3f, 0.3f, 0.3f, 0.75f);

  c[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.75f);
  c[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.3f, 0.0f, 0.9f);
  c[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);

  c[ImGuiCol_ScrollbarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);
  c[ImGuiCol_ScrollbarGrab] = ImVec4(0.2f, 0.2f, 0.2f, 0.9f);
  c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3f, 0.3f, 0.3f, 0.9f);
  c[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4f, 0.4f, 0.4f, 0.9f);

  c[ImGuiCol_CheckMark] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
  c[ImGuiCol_SliderGrab] = ImVec4(0.0f, 0.4f, 0.0f, 0.9f);
  c[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.5f, 0.0f, 0.9f);

  c[ImGuiCol_Button] = ImVec4(0.0f, 0.3f, 0.0f, 0.9f);
  c[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.55f, 0.0f, 0.9f);
  c[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.5f, 0.0f, 0.9f);

  c[ImGuiCol_Header] = ImVec4(0.5f, 0.0f, 1.0f, 0.5f);
  c[ImGuiCol_HeaderHovered] = ImVec4(0.5f, 0.0f, 1.0f, 0.8f);
  c[ImGuiCol_HeaderActive] = ImVec4(0.5f, 0.0f, 1.0f, 0.7f);

  c[ImGuiCol_Tab] = ImVec4(0.0f, 0.3f, 0.0f, 0.8f);
  c[ImGuiCol_TabHovered] = ImVec4(0.0f, 0.4f, 0.0f, 0.8f);
  c[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4f, 0.0f, 0.8f);
  c[ImGuiCol_TabUnfocused] = ImVec4(0.2f, 0.2f, 0.2f, 0.9f);
  c[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.9f);

  ImGui_ImplGlfw_InitForOpenGL(Window::m_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void ImGuiWrapper::Begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiWrapper::End() {
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiWrapper::Clean() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

}  // namespace saber
