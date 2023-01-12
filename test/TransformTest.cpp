#include <gtest/gtest.h>
#include <component/Transform.h>

using namespace component;


void match_mat(const glm::mat4& t, const glm::mat4& s) {

  for (int i = 0; i < 4;  i++) {
    for (int j =0; j < 4;  j++) {
      EXPECT_NEAR(t[i][j], s[i][j], 1e-4) << "index" << i << " " << j << std::endl;
    }
  }
}

void match_vec3(const glm::vec3& t, const glm::vec3& s) {
  for (int i = 0; i < 3; i++) {
    EXPECT_NEAR(t[i], s[i], 1e-4) << "index" << i << std::endl;
  }
}

TEST(TestUiteTransform, SetPosition) {
  Transform T;
  T.set_position(glm::vec3(1.0, 2.0, 3.0));
  // EXPECT_EQ(T.get_position(), glm::vec3(1.0, 2.0, 3.0));

  match_vec3(T.get_position(), glm::vec3(1.0, 2.0, 3.0));

  T.set_position(glm::vec3(10.0, -3.0, 7.0));
  // EXPECT_EQ(T.get_position(), glm::vec3(10.0, -3.0, 7.0));
  
  match_vec3(T.get_position(), glm::vec3(10.0, -3.0, 7.0));

  T.translate(glm::vec3(-1.0, -1.0, -1.0));
  // EXPECT_EQ(T.get_position(), glm::vec3(9.0, -4.0, 6.0));

  match_vec3(T.get_position(), glm::vec3(9.0, -4.0, 6.0));

}


TEST(TestUiteTransform, SetRotation) {

  Transform T;
  T.set_ealar_YZX(glm::vec3(30, 40, 50));
  auto t_ealer = T.get_eular();
  auto s_ealer = glm::vec3(30.0, 40.0, 50.0);

  for (int i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(t_ealer[i], s_ealer[i]);
  }

  auto t = T.get_transform();
  auto s = glm::mat4(0.4924039, 0.7660444, -0.4131759, 0.0, -0.1868108, 0.5566704, 0.8094565, 0.0, 0.8500825,  -0.3213938, 0.4172120, 0.0, 0.0, 0.0, 0.0, 1.0);

  for (int i = 0; i < 4;  i++) {
    for (int j =0; j < 4;  j++) {
      EXPECT_NEAR(t[i][j], s[i][j], 1e-4) << "index" << i << " " << j << std::endl;
    }
  }

  T.set_ealar_YZX(glm::vec3(-30, -10, 10));

  t= T.get_transform();
  s = glm::mat4(0.9698463, 0.1736482, 0.1710101, 0.0, -0.0612750, 0.8528686, -0.5185177, 0.0, -0.2358888, 0.4924039, 0.8377917, 0.0, 0.0, 0.0, 0.0, 1.0);

  t_ealer = T.get_eular();
  s_ealer = glm::vec3(-30.0, -10.0, 10.0);

  for (int i = 0; i < 3; i++) {
    EXPECT_FLOAT_EQ(t_ealer[i], s_ealer[i]);
  }


  for (int i = 0; i < 4;  i++) {
    for (int j =0; j < 4;  j++) {
      EXPECT_NEAR(t[i][j], s[i][j], 1e-4) << "index" << i << " " << j << std::endl;
    }
  }

}



TEST(TestUiteTransform, SetScale){
  Transform T;
  
  match_mat(T.get_transform(), glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0));

  T.set_scale(glm::vec3(2.0, 3.0, 4.0));
  
  auto t = T.get_transform();

  auto s = glm::mat4(2.0, 0.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0);

  match_mat(t, s);

  T.set_scale(glm::vec3(-2.0, 3.0, 0.0));

  t = T.get_transform();

  s = glm::mat4(-2.0, 0.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

  match_mat(t, s);
}

TEST(TestUiteTransform, SetTransform) {
  Transform T;

  T.set_ealar_YZX(glm::vec3(-10.0, 20.0, 40.0));

  auto temp = T.get_transform();

  T.set_position(glm::vec3(20.0, 100.0, -10.0));

  T.set_scale(glm::vec3(30.0, 20.0, 10.0));

  auto t = T.get_transform();

  auto s =  glm::mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 20.0, 100.0, -10.0, 1.0) * 
    temp * glm::mat4(30.0, 0.0, 0.0, 0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0);

  match_mat(t, s);
}

int main(int argc, char* argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
