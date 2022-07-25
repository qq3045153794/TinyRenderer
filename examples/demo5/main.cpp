#include <iostream>
#include <memory>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "asset/Buffer.h"
#include "asset/Shader.h"
#include "asset/Texture.h"
#include "asset/VAO.h"


const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

using namespace asset;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  GLfloat data[6][5] = {{0.0, 0.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0, 0.0},
                        {1.0, 0.0, 0.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0, 1.0},
                        {0.0, 1.0, 0.0, 0.0, 0.0}, {1.0, 1.0, 0.0, 1.0, 0.0}};
  std::unique_ptr<VBO> vbo(std::make_unique<VBO>(sizeof(data), data, GL_STATIC_DRAW));
  std::unique_ptr<VAO> vao(std::make_unique<VAO>());

  vao->set_vbo(*vbo, 0U, 3U, sizeof(GLfloat) * 5U, 0U, GL_FLOAT);
  vao->set_vbo(*vbo, 1U, 2U, sizeof(GLfloat) * 5U, 3U * sizeof(GLfloat), GL_FLOAT);
  
  

  std::unique_ptr<Shader> shader(
    std::make_unique<Shader>("../examples/demo5/image.vs", "../examples/demo5/image.fs"));

  std::unique_ptr<Texture> texture(std::make_unique<Texture>("../resource/texture/awesomeface.png"));
  std::vector<GLuint> offset = {0, 64U};
  std::vector<GLuint> lenght = {64U, 64U};
  
  GLuint sz = 128;
  std::unique_ptr<UBO> ubo(std::make_unique<UBO>(offset, lenght,sz));
  ubo->set_binding(0, "Matrices", shader->get_id());
  
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->bind();
    
    glm::mat4 projection = glm::ortho(0.0f, (GLfloat)SCREEN_WIDTH, 0.0f, (GLfloat)SCREEN_HEIGHT, -1.0f, 1.0f);
    // shader->set_uniform("projection", projection);
    glm::mat4 view(1.0);
    // shader->set_uniform("view", view);
    
    ubo->set_uniform(0, glm::value_ptr(projection));
    ubo->set_uniform(1, glm::value_ptr(view));
    
    glm::mat4 model(1.0);
    model = glm::scale(model, glm::vec3(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0));
    shader->set_uniform("model", model);
    shader->set_uniform("texture_1", 0);

    texture->bind(0);
    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    vao->ubind();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}