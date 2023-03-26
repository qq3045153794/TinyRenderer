#ifndef _SRC_MANAGE_ASSET_MANAGE_H_
#define _SRC_MANAGE_ASSET_MANAGE_H_
#include <manage/BaseManage.h>
#include <yaml-cpp/yaml.h>

namespace saber {

class AssetManage : public BaseManage {
 public:
  virtual void Init() override;
  virtual void SaveConfig() override;
  virtual void Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) override;
 private:
};

}  // namespace saber

#endif
