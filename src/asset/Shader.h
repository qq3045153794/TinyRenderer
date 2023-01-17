#ifndef _SRC_ASSET_SHADER_H_
#define _SRC_ASSET_SHADER_H_
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
  
  Shader(){};
  ~Shader();
  Shader(const GLchar* vertex_file, const GLchar* fragment_file, const GLchar* geometry_file);
  Shader(const GLchar* vertex_file, const GLchar* fragment_file);
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  // 设置uniform值
  template <typename T>
  void set_uniform(const GLchar* name, const T& val);
  const GLuint get_id() const {return m_id;}

  void bind() const;
  void ubind() const;

 private:
  GLuint m_id;
  GLuint create_shader(const GLchar* code, GLuint type);
  void check_compile_errors(GLuint object, const std::string& type);

  void compile(const GLchar* vertex_code, const GLchar* fragment_code, const GLchar* geometry_code);
  void compile(const GLchar* vertex_code, const GLchar* fragment_code);
};

}  // namespace asset

#endif
