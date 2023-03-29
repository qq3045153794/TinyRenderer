#include <library/TextureLibrary.h>
#include <manage/ConfigManage.h>
namespace saber {

using Texture = asset::Texture;
Library<asset::Texture>::Library() {
  const uint32_t kLowResolution = 128U;
  const uint32_t kResolution = 512U;
  auto& texture_path = PublicSingleton<ConfigManage>::GetInstance().system_texture_path;
  // clang-format off
  Add("irradian", std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, kLowResolution, kLowResolution, GL_RGB));
  Add("hdr", std::make_shared<Texture>((texture_path / "hotel_room_4k2.hdr").c_str(), static_cast<int32_t>(kResolution)));
  Add("BRDF_LUT", std::make_shared<Texture>(GL_TEXTURE_2D, kResolution, kResolution, GL_RG16F));
  Add("default", std::make_shared<Texture>((texture_path / "003.jpeg").c_str(), true, 7));
  Add("prefiltermap", std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, kLowResolution, kLowResolution, GL_FLOAT, 5));
  //clnag-format on

  // add icons
  auto& icons_path = PublicSingleton<ConfigManage>::GetInstance().icons_path;
  Add("add icon", std::make_shared<Texture>((icons_path / "add_icon.png").c_str(), true, 7));
  Add("folder icon", std::make_shared<Texture>((icons_path / "folder_icon.png").c_str(), true, 7));
  Add("picture icon", std::make_shared<Texture>((icons_path / "picture_icon.png").c_str(), true, 7));
  Add("txt file icon", std::make_shared<Texture>((icons_path / "txt_file_icon.png").c_str(), true, 7));
  Add("rabbit icon", std::make_shared<Texture>((icons_path / "rabbit_icon.png").c_str(), true, 7));
}

std::shared_ptr<Texture> Library<asset::Texture>::GetDefaultTexture() {
  return m_library["default"];
}

std::shared_ptr<Texture> Library<asset::Texture>::GetWhiteTexture() {
  return m_library["default"];
}

std::shared_ptr<asset::Texture> Library<asset::Texture>::GetAddIcon() {
  return m_library["add icon"];
}

std::shared_ptr<asset::Texture> Library<asset::Texture>::GetFolderIcon() {
  return m_library["folder icon"];
}

std::shared_ptr<asset::Texture> Library<asset::Texture>::GetPictureIcon() {
  return m_library["picture icon"];
}

std::shared_ptr<asset::Texture> Library<asset::Texture>::GetTxtFileIcon() {
  return m_library["txt file icon"];
}

std::shared_ptr<asset::Texture> Library<asset::Texture>::GetRabbitIcon() {
  return m_library["rabbit icon"];
}

}  // namespace saber
