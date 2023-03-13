#include <library/TextureLibrary.h>
namespace saber {

using Texture = asset::Texture;
Library<asset::Texture>::Library() {
  const uint32_t kLowResolution = 32U;
  const uint32_t kResolution = 512U;
  // clang-format off
  Add("irradian", std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, kLowResolution, kLowResolution, GL_RGB));
  Add("hdr", std::make_shared<Texture>("../resource/texture/hotel_room_4k2.hdr", static_cast<int32_t>(kResolution)));
  Add("BRDF_LUT", std::make_shared<Texture>(GL_TEXTURE_2D, kResolution, kResolution, GL_RG16F));
  Add("default", std::make_shared<Texture>("../resource/texture/003.jpeg", true, 7));
  Add("prefiltermap", std::make_shared<Texture>(GL_TEXTURE_CUBE_MAP, kLowResolution, kLowResolution, GL_FLOAT, 5));
  //clnag-format on

}
std::shared_ptr<Texture> Library<asset::Texture>::GetDefaultTexture() {
  return m_library["default"];
}

std::shared_ptr<Texture> Library<asset::Texture>::GetWhiteTexture() {
  // TODO
  return m_library["default"];
}

}  // namespace saber
