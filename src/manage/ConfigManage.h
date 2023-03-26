#ifndef _SRC_MANAGE_CONFIG_MANAGE_H_
#define _SRC_MANAGE_CONFIG_MANAGE_H_

#include <manage/BaseManage.h>

namespace saber {

class ConfigManage : public BaseManage {
 public:
  virtual void Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) override;
  // 暂时相对路径
  std::filesystem::path icons_path {"resource/config/icons"};
};

}  // namespace saber

#endif
