#include <manage/BaseManage.h>

namespace saber {

const std::vector<std::filesystem::path>& BaseManage::GetResource() const { return m_resource_storage; }


bool BaseManage::CheckResExit(const std::filesystem::path& flie_path) {
  return m_resource_register.count(flie_path) > 0;
}

}  // namespace saber
