#include <iostream>
#include "component/Transform.h"
#include <memory>

int main() {

  std::shared_ptr<component::Transform> T =std::make_shared<component::Transform>();
  auto& pos =  T->get_position();
  std ::cout << pos.x << pos.y << pos.z << std::endl;
}
