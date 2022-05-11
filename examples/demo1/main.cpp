#include "ResourceManager.h"
#include "Trangle.h"

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
Trangle* trangle;
void init() {
  Shader shander_trangle = ResourceManager::load_shader("", "","trangle");
  Texture2D texture_2d_trangle = ResourceManager::load_texture("", "trangle");
  trangle = new Trangle(ResourceManager::get_shader("trangle"), ResourceManager::get_texture("trangle"));
  ResourceManager::get_shader("trangle").set_matrix4("projection", glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f), true);
  ResourceManager::get_shader("trangle").set_matrix4("view", glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -3.0)),true);
}
int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  while (!glfwWindowShouldClose(window)){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    trangle->draw();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}