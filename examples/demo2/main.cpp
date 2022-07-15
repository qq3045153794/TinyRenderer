
#include <memory>

#include "Image.h"
#include "ResourceManager.h"
#include "YUVFrame.h"
using YUVFrame = gl_frame::YUVFrame;
std::shared_ptr<Image> image_render;
std::shared_ptr<YUVFrame> yuv_frame;
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 800;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void init() {
  image_render = Image::create_image_model(glm::vec2(500.0, 500.0),
                                           glm::vec3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5, 0.0),
                                           glm::quat(1.0, 0.0, 0.0, 0.0));
  yuv_frame = std::shared_ptr<YUVFrame>(new YUVFrame(SCREEN_WIDTH, SCREEN_HEIGHT, 4));

  ResourceManager::load_shader("../examples/demo2/image.vs", "../examples/demo2/image.fs",
                               "image_shader");
  ResourceManager::get_shader("image_shader").use();
  glm::mat4 projection =
      glm::ortho(0.0f, (GLfloat)SCREEN_WIDTH, 0.0f, (GLfloat)SCREEN_HEIGHT, -1.0f, 1.0f);
  glm::mat4 view = glm::mat4(1.0);

  ResourceManager::get_shader("image_shader").set_matrix4("projection", projection);
  ResourceManager::get_shader("image_shader").set_matrix4("view", view);

  ResourceManager::get_shader("image_shader").set_integer("texture_0", 0);
  ResourceManager::load_texture("../resource/texture/001.jpg", "001");
  glActiveTexture(GL_TEXTURE0);
  ResourceManager::get_texture("001").bind();
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // 窗口禁止拖动
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  init();
  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    processInput(window);

    yuv_frame->begin_render();
    image_render->draw(ResourceManager::get_shader("image_shader"));
    yuv_frame->end_render();

    yuv_frame->render();
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}