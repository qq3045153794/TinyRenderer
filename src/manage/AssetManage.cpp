#include <core/Debug.h>
#include <manage/AssetManage.h>
#include <manage/ConfigManage.h>
#include <utils/File.h>

namespace saber {

void AssetManage::Init() {
  auto path = PublicSingleton<ConfigManage>::GetInstance().config_path / "asset_path.yml";
  if (std::filesystem::exists(path)) {
    DeSerialize(path);
    // 检查序列化的文件是否存在与磁盘中
    if (!CheckPathExit()) {
      return throw std::runtime_error("The loaded file does not exist on disk (file = asset_path.yml)");
    }


  } else {
    CORE_WARN("Asset Path no exists");
  }
}


// AssetManage::~AssetManage() {
//  SPDLOG_DEBUG("Call ~AssetManage");
//  textures_cache.clear();
// }

void AssetManage::Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) {

  CORE_ASERT(!std::filesystem::is_directory(from_path), "From path is Dir!");
  CORE_ASERT(std::filesystem::is_directory(to_path), "From path is not Dir!");

  std::string expend_meta = to_path.string() + "/" + from_path.filename().string() + ".extend";
  std::filesystem::path import_name_path = to_path.string() + "/" + from_path.filename().string();

  // 已经导入过 skip
  if (m_resource_register.count(import_name_path) > 0) {
    return;
  }

  CORE_DEBUG("from_path : {} to_path : {}", from_path.string(), to_path.string());
  if (!std::filesystem::equivalent(from_path.parent_path(), to_path)) {
    std::filesystem::copy(from_path, to_path, std::filesystem::copy_options::overwrite_existing);
  }

  // 判断重复
  if (m_resource_register.count(import_name_path) <= 0) {
    m_resource_storage.emplace_back(import_name_path);
    m_resource_register[import_name_path] = std::filesystem::path(expend_meta);
  }

  // 加载份扩展解释该文件
  // 包含 MD5
  // 该资源引用的MD5与文件绝对路径的映射
  YAML::Emitter meta_out;
  meta_out << YAML::BeginMap;
  meta_out << YAML::Key << "file path name";
  meta_out << YAML::Value << import_name_path.string();
  meta_out << YAML::EndMap;
  ::utils::File::write_yml_file(expend_meta, meta_out);

  SaveConfig();
}

void AssetManage::Remove(const std::filesystem::path& filename_path) {
  CORE_ASERT(m_resource_register.count(filename_path) > 0, "No find filename (filename_path = {})", filename_path.string());
  auto find_item = std::find(m_resource_storage.begin(), m_resource_storage.end(), filename_path);
  CORE_ASERT(find_item != m_resource_storage.end(), "No find filename (filename_path = {})", filename_path.string());
  m_resource_storage.erase(find_item);
  m_resource_register.erase(filename_path);
  SaveConfig();
}

void AssetManage::SaveConfig() {
  auto path = PublicSingleton<ConfigManage>::GetInstance().config_path / "asset_path.yml";
  Serialize(path);
}




}  // namespace saber
