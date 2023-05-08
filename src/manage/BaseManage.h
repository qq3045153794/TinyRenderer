#ifndef _SRC_MANAGE_BASEMANAGE_H_
#define _SRC_MANAGE_BASEMANAGE_H_
#include <core/PublicSingleton.h>

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
#include <yaml-cpp/yaml.h>
#include <core/Log.h>
namespace saber {
class BaseManage : public PublicSingleton<BaseManage> {
 public:
  virtual void Init() = 0;
  virtual void SaveConfig() = 0;
  virtual void Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) = 0;
  virtual void Remove(const std::filesystem::path& filename_path) = 0;
  virtual ~BaseManage() override = default;

  const std::vector<std::filesystem::path>& GetResource() const;
  bool CheckResExit(const std::filesystem::path& flie_path);

  std::vector<std::filesystem::path> m_resource_storage;
  std::map<std::filesystem::path, std::filesystem::path> m_resource_register;
 protected:
  void Serialize(const std::filesystem::path& file_name);
  void DeSerialize(const std::filesystem::path& file_name);
  bool CheckPathExit();

};
};  // namespace saber

#endif
