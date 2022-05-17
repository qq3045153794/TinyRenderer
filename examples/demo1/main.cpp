#include "ResourceManager.h"
#include "Trangle.h"
#include "RenderText.h"
#include <iostream>
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
Trangle* trangle;
RenderText* text;
void init() {
  ResourceManager::load_shader("../resource/shader/trangle.vs", "../resource/shader/trangle.fs","trangle");
  ResourceManager::load_texture("../resource/texture/awesomeface.png", "trangle");
  ResourceManager::load_shader("../resource/shader/front.vs", "../resource/shader/front.fs", "front");
  
  ResourceManager::get_shader("trangle").set_matrix4("projection", glm::perspective(glm::radians(60.0f), 
                (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f), true);
  ResourceManager::get_shader("trangle").set_matrix4("view", 
                 glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -3.0)), true);
  ResourceManager::get_shader("front").set_matrix4("projection", glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH), 0.0f, static_cast<GLfloat>(SCREEN_HEIGHT)), true);
  
  trangle = new Trangle(ResourceManager::get_shader("trangle"), ResourceManager::get_texture("trangle"));
  text = new RenderText(ResourceManager::get_shader("front"));
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
  init();
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  while (!glfwWindowShouldClose(window)){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    trangle->draw();
    glDisable(GL_DEPTH_TEST);
    text->draw(L"东风谷早苗",650.0, 25.0, glm::vec3(1.0, 1.0, 1.0));
    glEnable(GL_DEPTH_TEST);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}