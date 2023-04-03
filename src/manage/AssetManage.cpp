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

void AssetManage::Import(const std::filesystem::path& from_path, const std::filesystem::path& to_path) {
  // TODO

  CORE_ASERT(!std::filesystem::is_directory(from_path), "From path is Dir!");
  CORE_ASERT(std::filesystem::is_directory(to_path), "From path is not Dir!");


  std::string expend_meta = to_path.string() + "/" + from_path.filename().string() + ".extend";
  std::filesystem::path import_name_path = to_path.string() + "/" + from_path.filename().string();
  // 复制份到目标路径

  CORE_DEBUG("from_path : {} to_path : {}", from_path.string(), to_path.string());
  if (!std::filesystem::equivalent(from_path.parent_path(), to_path)) {
    std::filesystem::copy(from_path, to_path, std::filesystem::copy_options::overwrite_existing);
  }
  m_resource_storage.emplace_back(import_name_path);
  m_resource_register[import_name_path] = std::filesystem::path(expend_meta);

  // 加载份扩展解释该文件
  // 包含 MD5
  // 该资源引用的MD5与文件绝对路径的映射
  YAML::Emitter meta_out;
  meta_out << YAML::BeginMap;
  meta_out << YAML::Key << "file path name";
  meta_out << YAML::Value << import_name_path.string();
  meta_out << YAML::EndMap;
  ::utils::File::write_yml_file(expend_meta, meta_out);


  auto asset_path = PublicSingleton<ConfigManage>::GetInstance().config_path / "asset_path.yml";

  BuildTexture(import_name_path, 7U, true);
  SaveConfig();
}

void AssetManage::SaveConfig() {
  auto path = PublicSingleton<ConfigManage>::GetInstance().config_path / "asset_path.yml";
  Serialize(path);
}


void AssetManage::BuildTexture(const std::filesystem::path& file_name_path, uint32_t level, bool is_filp) {
  CORE_INFO("Build texture...(path = {})", file_name_path.string());
  textures_cache[file_name_path.string()] = std::make_shared<::asset::Texture>(file_name_path.c_str(), is_filp, level);
}

}  // namespace saber
