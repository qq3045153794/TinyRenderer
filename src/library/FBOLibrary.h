#ifndef _SRC_LIBRARY_FBOLIBRARY_H_
#define _SRC_LIBRARY_FBOLIBRARY_H_

#include <asset/FBO.h>
#include <library/Library.h>
namespace saber {

template <>
class Library<asset::FBO> : public LibraryBase<Library, asset::FBO> {
 public:
  Library();
  [[nodiscard]] std::shared_ptr<asset::FBO> GetMainFBO();
  [[nodiscard]] std::shared_ptr<asset::FBO> GetShadowFBO();
  [[nodiscard]] std::shared_ptr<asset::FBO> GetDeBugShadowFBO();
};
}
#endif

