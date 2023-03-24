#ifndef _SRC_MANAGE_BASEMANAGE_H_
#define _SRC_MANAGE_BASEMANAGE_H_
#include <core/PublicSingleton.h>

#include <string>
#include <vector>
#include <filesystem>
namespace saber {
class BaseManage : public PublicSingleton<BaseManage> {
 public:
  std::string root_path;
  virtual void Import(const std::string& from_path, const std::string& to_path) = 0;
  const std::vector<std::string>& GetResource() const;

 protected:
  std::vector<std::string> m_resource_storage;
};
};  // namespace saber

#endif
