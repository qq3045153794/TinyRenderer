#include <gtest/gtest.h>
#include <component/Transform.h>

using namespace component;

TEST(TestUiteTransform, Position) {
  Transform T;
  T.set_position(glm::vec3(1.0, 2.0, 3.0));
  EXPECT_EQ(T.get_position(), glm::vec3(1.0, 2.0, 3.0));

  T.set_position(glm::vec3(10.0, -3.0, 7.0));
  EXPECT_EQ(T.get_position(), glm::vec3(10.0, -3.0, 7.0));

  T.translate(glm::vec3(-1.0, -1.0, -1.0));
  EXPECT_EQ(T.get_position(), glm::vec3(9.0, -4.0, 6.0));
}


TEST(TestUiteTransform, Rotation) {
  Transform T;
  T.set_ealar_YZX(glm::vec3(30, 40, 50));
  EXPECT_EQ(T.get_eular(), glm::vec3(30.0, 40.0, 50.0));
  EXPECT_EQ(T.get_transform(), glm::mat4(0.4508776, 0.8281304, -0.3330308, 0.0, -0.5066928, 0.5446302, 0.6683115, 0.0,  0.7348278, -0.1325824, 0.6651692, 0.0, 0.0, 0.0, 0.0, 1.0));

}
int main(int argc, char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
