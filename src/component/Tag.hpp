#ifndef _SRC_COMPONENT_TAG_H_
#define _SRC_COMPONENT_TAG_H_
#include <string>
namespace component {

enum ETag {
  Untagged = 1 << 0,
  Static = 1 << 1,
  MainCamera = 1 << 2,
  WorldPlane = 1 << 3,
  Skybox = 1 << 4,
  Water = 1 << 5,
  Particle = 1 << 6
};

class Tag {
 public:
  Tag(ETag tag, const std::string& name) : m_name(name),  m_tag(tag) {}
  void add(ETag tag) {
    m_tag = static_cast<ETag>(static_cast<int>(m_tag) | static_cast<int>(tag));
  }
  void del(ETag tag) {
    m_tag =
        static_cast<ETag>(static_cast<int>(m_tag) & (~static_cast<int>(tag)));
  }

  constexpr bool contains(ETag tag) const {
    return (static_cast<int>(m_tag) & static_cast<int>(tag)) > 0;
  }
  std::string m_name;
  ETag m_tag;
};

}  // namespace component

#endif
