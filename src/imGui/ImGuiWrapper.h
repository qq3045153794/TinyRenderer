
#ifndef _SRC_IMGUI_IMGUI_WRAPPER_H
#define  _SRC_IMGUI_IMGUI_WRAPPER_H
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

};

}
#endif