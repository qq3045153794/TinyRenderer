#include <iostream>
#include "component/Transform.h"
#include <memory>
#include "gtest/gtest.h"
int main() {
  EXPECT_EQ(1, 1) << "ok";
  EXPECT_EQ(1, 2) << "faid";
  std::shared_ptr<component::Transform> T =std::make_shared<component::Transform>();
  auto& pos =  T->get_position();
  std ::cout << pos.x << pos.y << pos.z << std::endl;
}
