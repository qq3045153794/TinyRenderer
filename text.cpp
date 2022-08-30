#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

    glm::mat4 lookat(const glm::vec3& eye, const glm::vec3& center, glm::vec3& world_up) {
        glm::vec3 z_aixs = glm::normalize(eye - center);
        glm::vec3 x_aixs = glm::normalize(glm::cross(glm::normalize(world_up), z_aixs));
        glm::vec3 y_aixs = glm::cross(z_aixs, x_aixs);

        glm::mat4 translate = glm::translate(glm::mat4(1.0), -eye);

        glm::mat4 view(1.0);
        view[0] = glm::vec4(x_aixs, 0.0);
        view[1] = glm::vec4(y_aixs, 0.0);
        view[2] = glm::vec4(z_aixs, 0.0);
        return glm::inverse(view) * translate; 
    }


int main() {
    glm::vec3 pos(1.0, 2.1, 3.2);
    glm::vec3 center(3.1, 2.1, 2.2);
    glm::vec3 up(0.0, 1.0, 0.0);

    glm::mat4 view_1 = lookat(pos, center, up);
    glm::mat4 view_2 = glm::lookAt(pos, center, up);

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout<<view_1[j][i]<<" ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout<<view_2[j][i]<<" ";
        }
        std::cout << std::endl;
    }
    
    glm::vec4 p(0.0, 0.0, 1.0, 1.0);
    glm::mat4 proj = glm::ortho(-1.0 , 1.0, -1.0, 1.0, 0.0 , 2.0);


    glm::vec4 t_p = view_1 * glm::vec4(pos, 1.0);

    glm::vec4 ndc_p = proj * p ;
    std::cout<< ndc_p.x << " " << ndc_p.y << " " << ndc_p.z << std::endl;
    std::cout<< t_p.x << " " << t_p.y << " " << t_p.z << " " << std::endl;

}