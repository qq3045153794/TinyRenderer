#ifndef _SRC_MANAGE_BASEMANAGE_H_
#define _SRC_MANAGE_BASEMANAGE_H_
#include <core/PublicSingleton.h>

#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>
namespace saber {
class BaseManage : public PublicSingleton<BaseManage> {
 public:
  std::filesystem::path root_path;
  virtual void Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) = 0;
  const std::vector<std::filesystem::path>& GetResource() const;
  bool CheckResExit(const std::filesystem::path& flie_path);

 protected:
  std::vector<std::filesystem::path> m_resource_storage;
  std::unordered_map<std::filesystem::path, std::filesystem::path> m_resource_register;
};
};  // namespace saber

#endif
