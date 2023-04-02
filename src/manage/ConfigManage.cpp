#include <manage/ConfigManage.h>
#include <core/App.h>

namespace saber {

void ConfigManage::Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) {
  // TODO
}

void ConfigManage::Init() {
  icons_path = core::App::instand().root_path + "/resource/config/icons";
  config_path = core::App::instand().root_path + "/resource/config/config";
  system_texture_path = core::App::instand().root_path + "/resource/config/textures";
  fonts_path = core::App::instand().root_path + "/resource/config/fonts";
  shader_path = core::App::instand().root_path + "/resource/config/shader";
  content_path = core::App::instand().root_path + "/resource/content";
  content_scenes_path = core::App::instand().root_path + "/resource/content/scenes";


}

}  // namespace saber
