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
  virtual ~AssetManage() override = default;
  bool IsPicture(const std::filesystem::path& filepath) const;
  bool Is3DModel(const std::filesystem::path& filepath) const;
  std::vector<std::filesystem::path> GetImageFilter() const;
  std::vector<std::filesystem::path> GetModelFilter() const;

 private:
};

}  // namespace saber

#endif
