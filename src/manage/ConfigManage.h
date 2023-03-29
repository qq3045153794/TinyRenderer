#ifndef _SRC_MANAGE_CONFIG_MANAGE_H_
#define _SRC_MANAGE_CONFIG_MANAGE_H_

#include <manage/BaseManage.h>

namespace saber {

class ConfigManage : public PublicSingleton<ConfigManage> {
 public:
  void Init();
  void Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path);
  // 暂时相对路径
  std::filesystem::path icons_path {"resource/config/icons"};
  std::filesystem::path config_path;
  std::filesystem::path resource_path;
  std::filesystem::path system_texture_path;
  std::filesystem::path fonts_path;
  std::filesystem::path shader_path;
  std::filesystem::path content_path;
};

}  // namespace saber

#endif
