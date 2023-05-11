#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

class TestTexture {
 public:
  GLuint m_id;
  void Init() {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 255, 255, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  //void Free() {
  //  glDeleteTextures(1, &m_id);
  //}
  ~TestTexture() {
    std::cout << "call TestTexture" << std::endl;
    glDeleteTextures(1, &m_id);
  }
};

int main(void) {
  // GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit()) return -1;

  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  auto t = std::make_shared<TestTexture>();
  t->Init();


  /* Loop until the user closes the window */

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  return 0;
}
