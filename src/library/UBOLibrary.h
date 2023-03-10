#ifndef _SRC_LIBRARY_UBO_LIBRARY_H_
#define _SRC_LIBRARY_UBO_LIBRARY_H_
#include <asset/Buffer.h>
#include <library/Library.h>
namespace saber {
template <>
class Library<asset::UBO> : public LibraryBase<Library, asset::UBO> {
 public:
  Library();

  [[nodiscard]] std::shared_ptr<asset::UBO> GetCameraUniformBuffer();
};
}  // namespace saber
#endif
