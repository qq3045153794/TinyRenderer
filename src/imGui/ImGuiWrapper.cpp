#include <core/Window.h>
#include <imGui/ImGuiWrapper.h>

namespace saber {

static const char* glsl_version = "#version 330";

static ImFont* truetype_font;  // TrueType, Lato-Regular, 18pt (main font)

void ImGuiWrapper::Init() {
  using Window = ::core::Window;

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
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
#if 0
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
#endif

  auto& colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

  // Headers
  colors[ImGuiCol_Header] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_HeaderHovered] = ImVec4{0.4f, 0.405f, 0.41f, 1.0f};
  colors[ImGuiCol_HeaderActive] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};

  // Buttons
  colors[ImGuiCol_Button] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_ButtonHovered] = ImVec4{0.4f, 0.405f, 0.41f, 1.0f};
  colors[ImGuiCol_ButtonActive] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};

  // Frame BG
  colors[ImGuiCol_FrameBg] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
  colors[ImGuiCol_FrameBgHovered] = ImVec4{0.4f, 0.405f, 0.41f, 1.0f};
  colors[ImGuiCol_FrameBgActive] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};

  // Tabs
  colors[ImGuiCol_Tab] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
  colors[ImGuiCol_TabHovered] = ImVec4{0.48f, 0.4805f, 0.481f, 1.0f};
  colors[ImGuiCol_TabActive] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
  colors[ImGuiCol_TabUnfocused] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};

  // Title
  colors[ImGuiCol_TitleBg] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
  colors[ImGuiCol_TitleBgActive] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.25f, 0.2505f, 0.251f, 1.0f};

  // Resize Grip
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

  // Scrollbar
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.53f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);

  // Check Mark
  colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);

  // Slider
  colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

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
