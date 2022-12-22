// clang-format off
#include "scene/ui.h"

#include <stdio.h>

#define IMGUI_DISABLE_METRICS_WINDOW
#define IMGUI_DEFINE_MATH_OPERATORS

#include "core/Log.h"
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
    ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

const char* glsl_version = "#version 130";

void init() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(Window::m_window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void new_frame() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
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

void draw_menu_bar() {
  ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
  ImGui::SetNextWindowSize(ImVec2((float)Window::m_width, 0.01f));
  ImGui::SetNextWindowBgAlpha(0.0f);

  // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
  // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10));
  // ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 10));
  //

  // ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 0.0f, 0.0f, 0.75f));
  // ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.55f));
  // ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.22f, 0.39f, 0.61f, 0.8f));
  // ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.24f, 0.54f, 0.89f, 0.8f));

  //

  ImGui::Begin("Menu Bar", 0,
               ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

  if (ImGui::BeginMenuBar()) {
    // ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.7f, 0.7f, 0.7f, 0.3f));
    //
    // ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.3f, 0.3f, 0.3f, 0.3f));

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
  }
  ImGui::End();
}

void draw_Gizmo(Entity& camera, Entity& target, Gizmo z) {
  static const ImVec2 win_pos = ImVec2(0.0f, 50.0f);
  static const ImVec2 win_size = ImVec2((float)Window::m_width, (float)Window::m_height - 82.0f);

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

  ImGuizmo::Manipulate(value_ptr(V), value_ptr(P), operation, mode, value_ptr(transform));

  ImGui::End();
}

}  // namespace scene::ui