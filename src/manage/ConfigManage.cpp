#include <manage/ConfigManage.h>
#include <core/App.h>

namespace saber {

void ConfigManage::Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) {
  // TODO
}

void ConfigManage::Init() {
  config_path = core::App::instand().root_path + "/resource/config/config";
}

}  // namespace saber
