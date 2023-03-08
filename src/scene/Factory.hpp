#ifndef _SRC_SCENE_FACTORY_HPP_
#define _SRC_SCENE_FACTORY_HPP_
#include "core/Log.h"
#include "examples/Scene1.hpp"
#include "scene/Scene.h"
namespace scene::factory {
inline const std::vector<std::string> titles{"Welcome Screen",
                                             "Tiled Forward Renderer"};

inline Scene* LoadScene(const std::string& title) {
  if (title == "Welcome Screen") return new Scene(title);
  if (title == "Tiled Forward Renderer") return new Scene1(title);

  CORE_ERROR("Scene \"{0}\" is not registered in the factory ...", title);
  std::cin.get();
  exit(EXIT_FAILURE);
}
}  // namespace scene::factory
#endif