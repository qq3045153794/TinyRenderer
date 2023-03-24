#include <manage/AssetManage.h>
#include <utils/File.h>

namespace saber {

void AssetManage::Import(const std::string& from_path, const std::string& to_path) {
  // TODO

  YAML::Emitter meta_out;
  meta_out << YAML::BeginMap;
  meta_out << YAML::Key << "self path";
  meta_out << YAML::Value << from_path;
  meta_out << YAML::EndMap;

  // 加载份扩展解释该文件
  // 包含 MD5
  // 该资源引用的MD5与文件绝对路径的映射
  std::string expend_meta = to_path + ".meta";
  ::utils::File::write_file(expend_meta, meta_out);
  // 复制份到目标路径
  std::filesystem::copy(from_path, to_path);
  m_resource_storage.emplace_back(to_path);
}

}  // namespace saber
