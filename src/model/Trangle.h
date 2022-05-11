#ifndef _TRANGLE_H_
#define _TRANGLE_H_
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ResourceManager.h"

class Trangle {
public:
  Trangle(Shader shader, Texture2D texture_2d);
  void set_buffers();
  void draw();
private:
  Shader shader;
  Texture2D texture_2d;
  glm::vec3 position;
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
};
#endif