#include "ResourceManager.h"
#include "Trangle.h"
#include "TrackBallControls.h"
#include "CameraFps.h"
#include "RenderText.h"
#include <iostream>
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
Trangle* trangle;
RenderText* text;
gl_cameras::Camera* camera_p;
gl_cameras::CameraFps* camera_fps;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void init() {
  ResourceManager::load_shader("../resource/shader/trangle.vs", "../resource/shader/trangle.fs","trangle");
  ResourceManager::load_texture("../resource/texture/awesomeface.png", "trangle");
  ResourceManager::load_shader("../resource/shader/front.vs", "../resource/shader/front.fs", "front");
  
  //camera_fps = new gl_cameras::CameraFps(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
  camera_p = new gl_cameras::Camera(45.0f, static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT), 0.1f, 1000.0f);
  camera_p->look_at(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
  
  ResourceManager::get_shader("trangle").set_matrix4("projection", camera_p->get_projection_mat(), true);
  
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
  //glfwSetCursorPosCallback(window, mouse_callback);
  //glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  init();
  
  gl_cameras::TrackBallControls::instance().init(window, SCREEN_WIDTH, SCREEN_HEIGHT);
  gl_cameras::TrackBallControls::instance().set_camera(camera_p);
  
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_BLEND); 
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  while (!glfwWindowShouldClose(window)){
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    gl_cameras::TrackBallControls::instance().update();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    ResourceManager::get_shader("trangle").use();
    ResourceManager::get_shader("trangle").set_matrix4("view", camera_p->get_view_mat(), true);
    std::cout<<"e="<<camera_p->get_eye().x<<" "<<camera_p->get_eye().y<<" "<<camera_p->get_eye().z<<std::endl;
    std::cout<<"c="<<camera_p->get_center().x<<" "<<camera_p->get_center().y<<" "<<camera_p->get_center().z<<std::endl;
    std::cout<<"u="<<camera_p->get_up().x<<" "<<camera_p->get_up().y<<" "<<camera_p->get_up().z<<std::endl;
    trangle->draw();
    glDisable(GL_DEPTH_TEST);
    text->draw(L"东风谷早苗",650.0, 25.0, glm::vec3(1.0, 1.0, 1.0));
    glEnable(GL_DEPTH_TEST);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  /*
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camera_fps->processKeyboard(gl_cameras::FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camera_fps->processKeyboard(gl_cameras::BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera_fps->processKeyboard(gl_cameras::LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera_fps->processKeyboard(gl_cameras::RIGHT, deltaTime);
  */
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{

    float xpos = static_cast<GLfloat>(xposIn);
    float ypos = static_cast<GLfloat>(yposIn);
    std::cout<<xpos<<" "<<ypos<<std::endl;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera_fps->processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera_fps->processMouseScroll(static_cast<float>(yoffset));
}
