#ifndef _SRC_SCENE_ENTITY_HPP_
#define _SRC_SCENE_ENTITY_HPP_

#include <entt/entt.hpp>
#include <string>

#include "component/Camera.h"
#include "component/Material.h"
#include "component/Mesh.h"
#include "component/Model.h"
#include "component/Transform.h"
#include "component/Tag.hpp"
#include "component/Light.h"
#include "component/Animator.h"
#include "core/Debug.h"
#include "core/Clock.h"
#include "utils/ext.hpp"

namespace scene {

class Entity {
 public:
  std::string name;
  entt::entity id = entt::null;

 private:
  entt::registry* registry = nullptr;

 public:
  Entity() = default;
  Entity(const std::string& name, entt::entity id, entt::registry* reg) : name(name), id(id), registry(reg) {}
  ~Entity() {}

  Entity(const Entity&) = default;
  Entity& operator=(const Entity&) = default;

  template <typename T, typename... Args>
  T& AddComponent(Args&&... args) {
    using namespace component;
    CORE_ASERT(!registry->all_of<T>(id), "{} already has component!", name);
    if constexpr (std::is_same_v<T, Camera> || std::is_same_v<T, CameraFps>) {
      auto& transform = registry->get<Transform>(id);
      return registry->emplace<T>(id, &transform, std::forward<Args>(args)...);
    } else {
      return registry->emplace<T>(id, std::forward<Args>(args)...);
    }
  }

  template <typename T>
  T& GetComponent() {
    return registry->get<T>(id);
  }

  template <typename T>
  bool Contains() {
    return registry->any_of<T>(id);
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
