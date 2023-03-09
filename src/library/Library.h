#ifndef _SRC_LIBRARY_LIBRARY_H_
#define _SRC_LIBRARY_LIBRARY_H_
#include <core/Log.h>
#include <core/PublicSingleton.h>

#include <memory>
#include <unordered_map>
namespace saber {
template <template <typename> typename Derived, typename LibType>
class LibraryBase : public PublicSingleton<Derived<LibType>> {
 public:
  void Add(const std::string& name, const std::shared_ptr<LibType>& mem) {
    CORE_ASERT(m_library.find(name) == m_library.end(),
               "Already have this member in Library...");
    m_library[name] = mem;
  }

  [[nodiscard]] std::shared_ptr<LibType> Get(const std::string& name) {
    CORE_ASERT(m_library.find(name) != m_library.end(),
               "Can't find this member in Library!");
    return m_library[name];
  }

 protected:
  std::unordered_map<std::string, std::shared_ptr<LibType>> m_library;
};

template <typename LibType>
class Library : public LibraryBase<Library, LibType> {
 public:
  Library();
};

}  // namespace saber

#endif
