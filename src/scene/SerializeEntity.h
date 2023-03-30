#ifndef _SRC_SCENE_SERIALIZE_ENTITY_
#define _SRC_SCENE_SERIALIZE_ENTITY_
#include <filesystem>
namespace scene {
class Entity;
class SerializeObject {
 public:
  static void SerializeEntity(const std::filesystem::path& file_name_path, Entity& entity);
  static void DeserializeEntity();
};
}  // namespace scene

#endif
