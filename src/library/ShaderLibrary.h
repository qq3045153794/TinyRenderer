#ifndef _SRC_LIBRARY_SHADER_LIBRARY_H_
#define _SRC_LIBRARY_SHADER_LIBRARY_H_
#include <asset/Shader.h>
#include <library/Library.h>
namespace saber {
using Shader = asset::Shader;
template <>
class Library<Shader> : public LibraryBase<Library, Shader> {
 public:
  Library();

  [[nodiscard]] std::shared_ptr<Shader> GetDefaultShader();
  [[nodiscard]] std::shared_ptr<Shader> GetPbrShader();
  [[nodiscard]] std::shared_ptr<Shader> GetSkyBoxShader();
};
}  // namespace saber
#endif
