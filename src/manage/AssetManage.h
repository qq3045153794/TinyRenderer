#ifndef _SRC_MANAGE_ASSET_MANAGE_H_
#define _SRC_MANAGE_ASSET_MANAGE_H_
#include <manage/BaseManage.h>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <asset/Texture.h>

namespace saber {

class AssetManage : public BaseManage {
 public:
  virtual void Init() override;
  virtual void SaveConfig() override;
  virtual void Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) override;
  virtual void Remove(const std::filesystem::path& filename_path) override;

  void BuildTexture(const std::filesystem::path& file_name_path, uint32_t level, bool is_filp);
  void DeleteTexture(const std::filesystem::path& file_name_path);
  std::unordered_map<std::string, std::shared_ptr<::asset::Texture>> textures_cache;

 private:
};

}  // namespace saber

#endif
