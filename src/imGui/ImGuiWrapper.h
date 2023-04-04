
#ifndef _SRC_IMGUI_IMGUI_WRAPPER_H
#define _SRC_IMGUI_IMGUI_WRAPPER_H

#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS

#include <ImGuiFileDialog.h>

#include <filesystem>
#include <optional>
#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif
namespace saber {

class ImGuiWrapper {
 public:
  static void Init();
  static void Begin();
  static void End();
  static void Clean();
  static void DrawCombo(const std::string& key, const std::vector<std::filesystem::path>&, std::optional<std::string>&,
                        std::function<void(const std::filesystem::path&)>);
  static std::optional<std::filesystem::path> DrawFileBrower(const std::string& key, bool& open, const char* filters);
  static std::optional<std::filesystem::path> DrawDirBrower(bool& open);
  static ImFont* config_font;
  static ImFont* main_font;
};

}  // namespace saber
#endif
