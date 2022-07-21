#ifndef _LEARN_OPENGL_SRC_ASSET_SHADER_H_
#define _LEARN_OPENGL_SRC_ASSET_SHADER_H_
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace asset {

class Shader {
 public:
  //着色器句柄
  GLuint m_id; 
  Shader() {};
  ~Shader();
  Shader(const GLchar* vertex_code, const GLchar* fragment_code, const GLchar* geometry_code);
  Shader(const GLchar* vertex_code, const GLchar* fragment_code);
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  // 设置uniform值
  template <typename T>
  void set_uniform(const GLchar* name, T& val);

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