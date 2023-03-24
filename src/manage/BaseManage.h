#ifndef _SRC_MANAGE_BASEMANAGE_H_
#define _SRC_MANAGE_BASEMANAGE_H_
#include <core/PublicSingleton.h>

#include <functional>
#include <string>
#include <vector>
namespace saber {
class BaseManage : public PublicSingleton<BaseManage> {
 public:
  std::string root_path;
  virtual void Import(const std::string& import_path) = 0;
  const std::vector<std::string>& GetResource() const;

 private:
  std::vector<std::string> m_resource_storage;
};
};  // namespace saber

#endif
