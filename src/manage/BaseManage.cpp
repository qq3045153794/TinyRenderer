#include <manage/BaseManage.h>

namespace saber {

const std::vector<std::filesystem::path>& BaseManage::GetResource() const { return m_resource_storage; }

}  // namespace saber
