#include <manage/AssetManage.h>

namespace saber {

void AssetManage::Import(const std::string& from_path, const std::string& to_path) {
  // TODO

  // 复制份到目标路径
  std::filesystem::copy(from_path, to_path);
  m_resource_storage.emplace_back(to_path);
}

}  // namespace saber
