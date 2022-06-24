#include <iostream>
#include <functional>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtc/constants.hpp"  // pi
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/norm.hpp"  
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
using namespace std;


int main() {
    glm::quat q = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
    std::cout<<q.w<<" "<<q.x<<" "<<q.y<<" "<<q.z<<std::endl;
    //q = glm::inverse(q);
    
    //std::cout<<q.w<<" "<<q.x<<" "<<q.y<<" "<<q.z<<std::endl;

    glm::vec3 vec = q * glm::vec3(1.0, 0.0, 0.0);
    
    
    std::cout<<vec.x<<" "<<vec.y<<" "<<vec.z<<"\n";

}