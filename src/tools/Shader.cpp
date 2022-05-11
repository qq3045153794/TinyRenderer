#include "Shader.h"

Shader &Shader::use()
{
  glUseProgram(this->ID);
  return *this;
}

void Shader::compile(const GLchar *vertex_code,
                     const GLchar *fragment_code)
{
  GLuint program = glCreateProgram();
  GLuint vs = create_shader(vertex_code, GL_VERTEX_SHADER);
  GLuint fs = create_shader(fragment_code, GL_FRAGMENT_SHADER);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  check_compile_errors(program, "PROGRAM");

  glDeleteShader(vs);
  glDeleteShader(fs);
  this->ID = program;
}

void Shader::compile(const GLchar *vertex_code,
                     const GLchar *fragment_code,
                     const GLchar *geometry_code)
{
  GLuint program = glCreateProgram();
  GLuint vs = create_shader(vertex_code, GL_VERTEX_SHADER);
  GLuint fs = create_shader(fragment_code, GL_FRAGMENT_SHADER);
  GLuint gs = create_shader(geometry_code, GL_GEOMETRY_SHADER);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glAttachShader(program, gs);
  glLinkProgram(program);

  check_compile_errors(program, "PROGRAM");

  glDeleteShader(vs);
  glDeleteShader(fs);
  glDeleteShader(gs);
  this->ID = program;
}

GLuint Shader::create_shader(const GLchar *code,
                             GLuint type)
{
  GLuint id = glCreateShader(type);
  glShaderSource(id, 1, &code, nullptr);
  glCompileShader(id);
  switch (type)
  {
  case GL_VERTEX_SHADER:
  {
    check_compile_errors(id, "Vertex Shader");
    break;
  }
  case GL_FRAGMENT_SHADER:
  {
    check_compile_errors(id, "Fragment Shader");
    break;
  }
  default:
    check_compile_errors(id, "Geometry Shader");
  }

  return id;
}

void Shader::check_compile_errors(GLuint object,
                                  const std::string &type)
{
  GLint success;
  if (type != "PROGRAM")
  {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      int len;
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &len);
      char *message = (char *)alloca(sizeof(char) * len);
      glGetShaderInfoLog(object, len, &len, message);
      std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n";
      std::cout << message << "\n";
    }
  }
  else
  {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success)
    {
      int len;
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &len);
      char *message = (char *)alloca(sizeof(char) * len);
      glGetShaderInfoLog(object, len, &len, message);
      std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n";
      std::cout << message << "\n";
    }
  }
}

void Shader::set_float(const GLchar *name,
                       GLfloat value,
                       GLboolean use_shader)
{
  if (use_shader)
    this->use();
  glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::set_integer(const GLchar *name,
                         GLint value,
                         GLboolean use_shader)
{
  if (use_shader)
    this->use();
  glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::set_vector2f(const GLchar *name,
                          GLfloat x,
                          GLfloat y,
                          GLboolean use_shader)
{
  if (use_shader)
    this->use();
  glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::set_vector2f(const GLchar *name,
                          const glm::vec2 &value,
                          GLboolean use_shader)
{
  if (use_shader)
    this->use();
  glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::set_vector3f(const GLchar *name,
                          GLfloat x,
                          GLfloat y,
                          GLfloat z,
                          GLboolean useShader)
{
  if (useShader)
    this->use();
  glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::set_vector3f(const GLchar *name,
                          const glm::vec3 &value,
                          GLboolean use_shader)
{
  if (use_shader)
    this->use();
  glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}

void Shader::set_vector4f(const GLchar *name,
                          GLfloat x,
                          GLfloat y,
                          GLfloat z,
                          GLfloat w,
                          GLboolean use_shader)
{
  if (use_shader)
    this->use();
  glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}

void Shader::set_vector4f(const GLchar *name,
                          const glm::vec4 &value,
                          GLboolean use_shader)
{
  if (use_shader)
    this->use();
  glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::set_matrix4(const GLchar *name,
                         const glm::mat4 &matrix,
                         GLboolean useShader)
{
  if (useShader)
    this->use();
  glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
