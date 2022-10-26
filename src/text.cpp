#include <iostream>
#include "glm/glm.hpp"
using namespace std;
int main() {

  
  float pitch = glm::radians(19.9829f), yaw = glm::radians(1.09665), roll=glm::radians(0.017382);

  //初始朝向
  glm::vec4 front = glm::vec4(-1.0, 0.0, 0.0, 0.0);
  
  //绕x轴旋转
  glm::mat4 R_roll = glm::mat4( 1.0,             0.0,            0.0,  0.0,
                                0.0, glm::cos(roll), -glm::sin(roll), 0.0,
                                0.0, glm::sin(roll),  glm::cos(roll), 0.0,
                                0.0,             0.0,            0.0, 1.0);
  //绕y轴旋转
  glm::mat4 R_pitch = glm::mat4(glm::cos(pitch), 0.0, glm::sin(pitch), 0.0,
                                        0.0,     1.0,             0.0, 0.0,
                              -glm::sin(pitch),  0.0, glm::cos(pitch), 0.0,
                                         0.0,    0.0,             0.0, 1.0);
  //绕z轴旋转
  glm::mat4 R_yaw = glm::mat4(glm::cos(yaw),  -glm::sin(yaw),   0.0, 0.0,
                              glm::sin(yaw),   glm::cos(yaw),   0.0, 0.0,
                                        0.0,             0.0,   1.0, 0.0,
                                        0.0,             0.0,   0.0, 1.0);
 
  glm::vec4 ans = R_roll * R_yaw  * R_pitch * front;
  cout<<ans.x<<" "<<ans.y<<" "<<ans.z<<" "<<"\n";
  std::cout<<(ans.x)*(ans.x)  + (ans.y)*(ans.y) + (ans.z)*(ans.z) << "\n"; 

}

/*
       x
       |
       |
       |
       |
y------------------
*/