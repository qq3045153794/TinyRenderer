#ifndef _SRC_SCENE_SERIALIZE_ENTITY_
#define _SRC_SCENE_SERIALIZE_ENTITY_
#include <filesystem>
#include <utils/File.h>
namespace scene {
class Entity;
class Scene;
class SerializeObject {
 public:
  static void SerializeEntity(const std::filesystem::path& file_name_path, Entity& entity);
  static Entity DeserializeEntity(const std::filesystem::path& file_name_path, Scene& scene);
  static void SerializeScene(const std::filesystem::path& file_name_path, Scene& scene);
  static void DeserializeScene(const std::filesystem::path& file_name_path, Scene& scene);
 private:
  static void SerializeEntity(Entity& entity, YAML::Emitter& out);
  static Entity DeserializeEntity(Scene& scene, const YAML::Node& doc);
};
}  // namespace scene

#endif
