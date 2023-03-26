#include <core/Debug.h>
#include <manage/AssetManage.h>
#include <manage/ConfigManage.h>
#include <utils/File.h>

namespace saber {

void AssetManage::Init() {
  auto path = PublicSingleton<ConfigManage>::GetInstance().config_path / "asset_path.yml";
  if (std::filesystem::exists(path)) {
    DeSerialize(path);
  } else {
    CORE_WARN("Asset Path no exists");
  }
}

void AssetManage::Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) {
  // TODO

  CORE_ASERT(!std::filesystem::is_directory(from_path), "From path is Dir!");
  CORE_ASERT(std::filesystem::is_directory(to_path), "From path is not Dir!");

  YAML::Emitter meta_out;
  meta_out << YAML::BeginMap;
  meta_out << YAML::Key << "self path";
  meta_out << YAML::Value << from_path;
  meta_out << YAML::EndMap;

  // 加载份扩展解释该文件
  // 包含 MD5
  // 该资源引用的MD5与文件绝对路径的映射
  std::string expend_meta = to_path.string() + "/" + from_path.filename().string() + ".meta";
  std::filesystem::path import_name_path = to_path.string() + "/" + from_path.filename().string();
  ::utils::File::write_yml_file(expend_meta, meta_out);
  // 复制份到目标路径
  std::filesystem::copy(
      from_path, to_path,
      std::filesystem::copy_options::skip_existing | std::filesystem::copy_options::overwrite_existing);
  m_resource_storage.emplace_back(import_name_path);
  m_resource_register[import_name_path] = std::filesystem::path(expend_meta);
}

void AssetManage::SaveConfig() {

  auto path = PublicSingleton<ConfigManage>::GetInstance().config_path / "asset_path.yml";
  Serialize(path);
}

}  // namespace saber
