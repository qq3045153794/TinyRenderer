#ifndef _LEARN_OPENGL_SRC_ASSET_SHADER_H_
#define _LEARN_OPENGL_SRC_ASSET_SHADER_H_
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils/File.h"

namespace asset {
using namespace utils;

class Shader {
 public:
  //着色器句柄
  GLuint m_id;
  Shader(){};
  ~Shader();
  Shader(const GLchar* vertex_file, const GLchar* fragment_file, const GLchar* geometry_file);
  Shader(const GLchar* vertex_file, const GLchar* fragment_file);
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  // 设置uniform值
  template <typename T>
  void set_uniform(const GLchar* name, T val);
  void set_matrix4(const GLchar* name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(this->m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
  }
  void bind() const;
  void ubind() const;

 private:
  GLuint create_shader(const GLchar* code, GLuint type);
  void check_compile_errors(GLuint object, const std::string& type);

  void compile(const GLchar* vertex_code, const GLchar* fragment_code, const GLchar* geometry_code);
  void compile(const GLchar* vertex_code, const GLchar* fragment_code);
};

}  // namespace asset

#endif