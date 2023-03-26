#ifndef _SRC_LIBRARY_TEXTURE_LIBRARY_H_
#define _SRC_LIBRARY_TEXTURE_LIBRARY_H_
#include <asset/Texture.h>
#include <library/Library.h>
namespace saber {

template <>
class Library<asset::Texture> : public LibraryBase<Library, asset::Texture> {
 public:
  Library();
  [[nodiscard]] std::shared_ptr<asset::Texture> GetDefaultTexture();
  [[nodiscard]] std::shared_ptr<asset::Texture> GetWhiteTexture();

  [[nodiscard]] std::shared_ptr<asset::Texture> GetAddIcon();
  [[nodiscard]] std::shared_ptr<asset::Texture> GetFolderIcon();
  [[nodiscard]] std::shared_ptr<asset::Texture> GetPictureIcon();
  [[nodiscard]] std::shared_ptr<asset::Texture> GetTxtFileIcon();
  [[nodiscard]] std::shared_ptr<asset::Texture> GetRabbitIcon();
};
}  // namespace saber
#endif
