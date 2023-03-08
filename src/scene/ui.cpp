// clang-format off
#include "scene/ui.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>

#define IMGUI_DISABLE_METRICS_WINDOW
#define IMGUI_DEFINE_MATH_OPERATORS

#include "core/Log.h"
#include "scene/Factory.hpp"
#include "scene/Render.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"
#define GL_SILENCE_DEPRECATION

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

// clang-format on

namespace scene::ui {

using namespace core;

static const ImGuiColorEditFlags color3_flags =
    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha;
static const ImGuiWindowFlags invisible_window_flags =
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_NoInputs;

const char* glsl_version = "#version 330";

ImFont* truetype_font;  // TrueType, Lato-Regular, 18pt (main font)

static ImVec2 window_center;
static ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
static ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
static ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
static ImVec4 blue = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
static ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);

void init() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  ImGuiStyle& style = ImGui::GetStyle();

  std::string ttf_main = "../resource/fronts/Lato.ttf";

  float font_main_sz = 18.0f;

  ImFontConfig config_main;
  config_main.PixelSnapH = true;
  config_main.OversampleH = 4;
  config_main.OversampleV = 4;
  config_main.RasterizerMultiply =
      1.2f;  // brighten up the font to make them more readable
  config_main.GlyphExtraSpacing.x = 0.0f;  // 字间距离

  truetype_font = io.Fonts->AddFontFromFileTTF(ttf_main.c_str(), font_main_sz,
                                               &config_main);

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

void new_frame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuizmo::BeginFrame();
}

void end_frame() {
  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void show_down() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void draw_menu_bar(std::string& new_title) {
  const auto& curr_scene_title = Render::get_scene()->m_title;

  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2((float)Window::m_width, 0.01f));
  ImGui::SetNextWindowBgAlpha(0.0f);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 10));
  //

  ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
  ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.55f));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.22f, 0.39f, 0.61f, 0.8f));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                        ImVec4(0.24f, 0.54f, 0.89f, 0.8f));

  //

  ImGui::Begin("Menu Bar", 0,
               ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration |
                   ImGuiWindowFlags_NoMove);

  if (ImGui::BeginMenuBar()) {
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.7f, 0.7f, 0.7f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_BorderShadow,
                          ImVec4(0.3f, 0.3f, 0.3f, 0.3f));

    if (ImGui::BeginMenu("Open")) {
      for (unsigned int i = 0; i < scene::factory::titles.size(); i++) {
        std::string title = scene::factory::titles[i];
        std::ostringstream id;
        id << " " << std::setfill('0') << std::setw(2) << i;
        bool selected = (curr_scene_title == title);

        if (ImGui::MenuItem((" " + title).c_str(), id.str().c_str(),
                            selected)) {
          if (!selected) {
            new_title = title;
          }
        }
      }
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Options")) {
      if (ImGui::BeginMenu(" Window Resolution")) {
        ImGui::MenuItem(" 1280 x 720", NULL, false);
        ImGui::MenuItem(" 1600 x 900", NULL, true);
        ImGui::MenuItem(" 1920 x 1080", NULL, false);
        ImGui::MenuItem(" 2560 x 1440", NULL, false);

        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
    ImGui::PopStyleColor(2);
  }

  ImGui::End();

  ImGui::PopStyleColor(4);
  ImGui::PopStyleVar(3);
}

void draw_Gizmo(Entity& camera, Entity& target, Gizmo z) {
  static const ImVec2 win_pos = ImVec2(0.0f, 50.0f);
  static const ImVec2 win_size =
      ImVec2((float)Window::m_width, (float)Window::m_height - 82.0f);

  ImGuizmo::MODE mode = ImGuizmo::MODE::LOCAL;
  ImGuizmo::OPERATION operation = ImGuizmo::OPERATION::TRANSLATE;

  switch (z) {
    case Gizmo::TRANSLATE: {
      operation = ImGuizmo::OPERATION::TRANSLATE;
      break;
    }
    case Gizmo::ROTATE: {
      operation = ImGuizmo::OPERATION::ROTATE;
      break;
    }
    case Gizmo::SCALE: {
      operation = ImGuizmo::OPERATION::SCALE;
      break;
    }
    case Gizmo::BOUNDS:
    case Gizmo::NONE:
    default: {
      return;
    }
  }

  auto& T = target.GetComponent<component::Transform>();
  auto& C = camera.GetComponent<component::CameraFps>();
  glm::mat4 V = C.get_view_mat();
  glm::mat4 P = C.get_projection_mat();

  glm::mat4 transform = T.get_transform();

  ImGui::SetNextWindowPos(win_pos);
  ImGui::SetNextWindowSize(win_size);
  ImGui::Begin("##Invisible Gizmo Window", 0, invisible_window_flags);
  ImGuizmo::SetOrthographic(true);
  ImGuizmo::SetDrawlist();
  ImGuizmo::SetRect(win_pos.x, win_pos.y, win_size.x, win_size.y);

  ImGuizmo::Manipulate(value_ptr(V), value_ptr(P), operation, mode,
                       value_ptr(transform));

  if (ImGuizmo::IsUsing()) {
    T.set_transform(transform);
  }

  ImGui::End();
}

void draw_welcome_scene(ImTextureID id) {
  ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
  const static float win_w = (float)Window::m_width;
  const static float win_h = (float)Window::m_height;
  draw_list->AddImage(id, ImVec2(0.0f, 0.0f), ImVec2(win_w, win_h));
}

void draw_loading_screen() {
  const static float win_w = (float)Window::m_width;
  const static float win_h = (float)Window::m_height;
  const static float bar_w = 268.0f;
  const static float bar_h = 80.0f;

  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2(win_w, win_h));
  ImGui::SetNextWindowBgAlpha(1.0f);

  // ImGui::PushFont(opentype_font);
  ImGui::Begin("Loading Bar", 0, ImGuiWindowFlags_NoDecoration);

  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize() * 1.3f,
                     ImVec2((win_w - bar_w) * 0.5f, (win_h - bar_h) * 0.5f),
                     ImGui::ColorConvertFloat4ToU32(yellow),
                     "LOADING, PLEASE WAIT ......");

  float x = 505.0f;  // not magic number, it's simple math!
  float y = 465.0f;  // not magic number, it's simple math!
  const static float size = 20.0f;
  float r, g, b;

  for (float i = 0.0f; i < 1.0f; i += 0.05f, x += size * 1.5f) {
    r = (i <= 0.33f) ? 1.0f : ((i <= 0.66f) ? 1 - (i - 0.33f) * 3 : 0.0f);
    g = (i <= 0.33f) ? i * 3 : 1.0f;
    b = (i > 0.66f) ? (i - 0.66f) * 3 : 0.0f;

    draw_list->AddTriangleFilled(
        ImVec2(x, y - 0.5f * size), ImVec2(x, y + 0.5f * size),
        ImVec2(x + size, y), IM_COL32(r * 255, g * 255, b * 255, 255.0f));
  }

  ImGui::End();
  // ImGui::PopFont();
}

}  // namespace scene::ui