#include <manage/AssetManage.h>
#include <utils/File.h>
#include <core/Debug.h>

namespace saber {

void AssetManage::Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) {
  // TODO

  std::cout << "?" << std::endl;
  CORE_ASERT(!std::filesystem::is_directory(from_path), "From path is Dir!");
  CORE_ASERT(std::filesystem::is_directory(to_path), "From path is not Dir!");

  std::cout << "?" << std::endl;
  YAML::Emitter meta_out;
  meta_out << YAML::BeginMap;
  meta_out << YAML::Key << "self path";
  meta_out << YAML::Value << from_path;
  meta_out << YAML::EndMap;

  std::cout << "?" << std::endl;
  // 加载份扩展解释该文件
  // 包含 MD5
  // 该资源引用的MD5与文件绝对路径的映射
  std::cout << "?" << std::endl;
  std::string expend_meta = to_path.string() + "/" + from_path.filename().string() + ".meta";
  ::utils::File::write_file(expend_meta, meta_out);
  // 复制份到目标路径
  std::cout << expend_meta << std::endl;
  std::filesystem::copy(from_path, to_path, std::filesystem::copy_options::skip_existing | std::filesystem::copy_options::overwrite_existing);
  std::cout << "?" << std::endl;
  m_resource_storage.emplace_back(to_path);
  m_resource_register[to_path] = std::filesystem::path(expend_meta);
}

}  // namespace saber
