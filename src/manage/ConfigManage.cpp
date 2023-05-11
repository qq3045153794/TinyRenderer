#include <manage/ConfigManage.h>
#include <core/App.h>
#include <utils/File.h>

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

  // 读取配置文件
  auto settings_file = config_path / "settings_file";
  YAML::Node doc;
  utils::File::parser_yml_file(settings_file, doc);
  enable_shadow = doc["enable_shadow"].as<bool>();
}


void ConfigManage::SaveConfigSettings() {
  YAML::Emitter out;

  out << YAML::BeginMap;
  out << YAML::Key << "enable_shadow" << YAML::Value << enable_shadow;
  out << YAML::EndMap;

  auto settings_file = config_path / "settings_file";
  utils::File::write_yml_file(settings_file, out);
}



}  // namespace saber
