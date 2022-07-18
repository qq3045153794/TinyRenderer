#include "ResourceManager.h"
#include "Trangle.h"
#include "TrackBallControls.h"
#include "CameraFps.h"
#include "YUVFrame.h"
#include "Animation.hpp"
#include "Animator.hpp"
#include "model.hpp"
#include "TexCubeRender.h"
#include <iostream>
#include <memory>
using YUVFrame = gl_frame::YUVFrame;
const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;
Trangle* trangle;
std::shared_ptr<YUVFrame> yuv_frame;
gl_cameras::Camera* camera_p;
gl_animation::Model* model;
std::shared_ptr<TexCubeRender> tex_cube_render;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void init() {
  yuv_frame = std::shared_ptr<YUVFrame>(new YUVFrame(SCREEN_WIDTH, SCREEN_HEIGHT, 4));
  // yuv_frame->set_process(gl_frame::GAUSSIAN_BLUR);

  tex_cube_render = std::shared_ptr<TexCubeRender>(new TexCubeRender());
  ResourceManager::load_shader("../examples/demo4/bm1.vs", "../examples/demo4/bm1.fs", "anim");
  model = new gl_animation::Model("../resource/objects/派蒙bk/untitled.obj","false");
  
  camera_p = new gl_cameras::Camera(45.0f, static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT), 0.1f, 1000.0f);
  camera_p->look_at(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));  
  
  ResourceManager::get_shader("anim").set_matrix4("projection", camera_p->get_projection_mat(), true);
}
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
    

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    yuv_frame->begin_render();
    
    gl_cameras::TrackBallControls::instance().update();
    ResourceManager::get_shader("anim").use();
    ResourceManager::get_shader("anim").set_vector3f("V", camera_p->get_eye());
    ResourceManager::get_shader("anim").set_integer("tex_cube", 0);
    glActiveTexture(GL_TEXTURE0);
    tex_cube_render->get_tex_cube().Bind();

    ResourceManager::get_shader("anim").set_matrix4("view", camera_p->get_view_mat(), true);
    glm::mat4 t_model = glm::mat4(1.0);
    t_model = glm::translate(t_model, glm::vec3(0.0f, -1.0f, 0.0f));
    t_model = glm::scale(t_model, glm::vec3(2.0f,2.0f, 2.0f));
    ResourceManager::get_shader("anim").set_matrix4("model", t_model, true);
    
    tex_cube_render->render(*camera_p);
    
    model->draw(ResourceManager::get_shader("anim"));
    yuv_frame->end_render();

    yuv_frame->render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}