#ifndef _LEARN_OPENGL_SRC_SCENE_ENTITY_HPP_
#define _LEARN_OPENGL_SRC_SCENE_ENTITY_HPP_

#include <entt/entt.hpp>
#include <string>

#include "component/Transform.h"
#include "component/Mesh.h"
#include "component/Material.h"
#include "component/Camera.h"

namespace scene {

class Entity {
 private:
  entt::registry* registry = nullptr;

 public:
  entt::entity id = entt::null;
  std::string name;

 public:
  Entity() = default;
  Entity(const std::string& name, entt::entity id, entt::registry* reg)
      : name(name), id(id), registry(reg) {}
  ~Entity(){};

  Entity(const Entity&) = default;
  Entity& operator=(const Entity&) = default;

  template <typename T, typename... Args>
  T& AddComponent(Args&&... args) {
    
    if constexpr (std::is_same_v<T, component::Camera>) {
      auto transform = registry->get<component::Transform> (id);
      return registry->emplace<T> (id, &transform, std::forward<Args>(args)...)
    }
    return registry->emplace<T> (id, std::forward<Args>(args)...);
  }

  template <typename T>
  T& GetComponent() {
    return registry->get<T>(id);
  }

  template <typename T, typename... Args>
  T& SetComponent(Args&&... args) {
    return registry->emplace_or_replace<T>(id, std::forward<Args>(args)...);
  }

  template <typename T>
  void RemoveComponent() {
    registry->remove<T>(id);
  }
};

}  // namespace scene

#endif