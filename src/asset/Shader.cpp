#include "asset/Shader.h"

#include "core/Debug.h"
namespace asset {

static GLuint curr_bound_program = 0;

Shader::Shader(const GLchar *vertex_file, const GLchar *fragment_file,
               const GLchar *geometry_file) {
  const std::string &vertex_code = File::read_file(vertex_file);
  const std::string &fragment_code = File::read_file(fragment_file);
  const std::string &geometry_code = File::read_file(geometry_file);
  compile(vertex_code.c_str(), fragment_code.c_str(), geometry_code.c_str());
}

Shader::Shader(const GLchar *vertex_file, const GLchar *fragment_file) {
  const std::string &vertex_code = File::read_file(vertex_file);
  const std::string &fragment_code = File::read_file(fragment_file);
  compile(vertex_code.c_str(), fragment_code.c_str());
}

Shader::~Shader() {
  this->ubind();
  glDeleteProgram(m_id);
}

void Shader::bind() const {
  if (curr_bound_program != m_id) {
    glUseProgram(m_id);
    curr_bound_program = m_id;
  }
}

void Shader::ubind() const {
  if (curr_bound_program == m_id) {
    glUseProgram(0);
    curr_bound_program = 0;
  }
}

void Shader::compile(const GLchar *vertex_code, const GLchar *fragment_code) {
  GLuint program = glCreateProgram();
  GLuint vs = create_shader(vertex_code, GL_VERTEX_SHADER);
  GLuint fs = create_shader(fragment_code, GL_FRAGMENT_SHADER);
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  check_compile_errors(program, "PROGRAM");

  glDeleteShader(vs);
  glDeleteShader(fs);
  this->m_id = program;
}

void Shader::compile(const GLchar *vertex_code, const GLchar *fragment_code,
                     const GLchar *geometry_code) {
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
  this->m_id = program;
}

GLuint Shader::create_shader(const GLchar *code, GLuint type) {
  GLuint m_id = glCreateShader(type);
  glShaderSource(m_id, 1, &code, nullptr);
  glCompileShader(m_id);
  switch (type) {
    case GL_VERTEX_SHADER: {
      check_compile_errors(m_id, "Vertex Shader");
      break;
    }
    case GL_FRAGMENT_SHADER: {
      check_compile_errors(m_id, "Fragment Shader");
      break;
    }
    default:
      check_compile_errors(m_id, "Geometry Shader");
  }

  return m_id;
}

void Shader::check_compile_errors(GLuint object, const std::string &type) {
  GLint success;
  if (type != "PROGRAM") {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
      int len;
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &len);
      char *message = reinterpret_cast<char *>(alloca(sizeof(char) * len));
      glGetShaderInfoLog(object, len, &len, message);
      CORE_ERROR("ERROR::SHADER: Compile-time error: Type: ");
      std::cout << message << "\n";
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
      int len;
      glGetShaderiv(object, GL_INFO_LOG_LENGTH, &len);
      char *message = reinterpret_cast<char *>(alloca(sizeof(char) * len));
      glGetShaderInfoLog(object, len, &len, message);
      CORE_ERROR("ERROR::Shader: Link-time error: Type: ");
      std::cout << message << "\n";
    }
  }
}

template <typename T>
void Shader::set_uniform(const GLchar *name, const T &val) {
  using namespace glm;
  GLuint block_idx = glGetUniformLocation(m_id, name);

  if (block_idx == -1) {
    CORE_ERROR("Can't find valid uniform location for {}", name);
    return;
  }

  if constexpr (std::is_same_v<T, GLint>) {
    glUniform1i(block_idx, val);
  } else if constexpr (std::is_same_v<T, bool>) {
    glUniform1i(block_idx, val);
  } else if constexpr (std::is_same_v<T, GLfloat>) {
    glUniform1f(block_idx, val);
  } else if constexpr (std::is_same_v<T, GLuint>) {
    // 转换成int,一般不会传uint进入shader sampler2D也是int类型
    glUniform1i(block_idx, static_cast<GLint>(val));
  } else if constexpr (std::is_same_v<T, vec2>) {
    glUniform2fv(block_idx, 1, glm::value_ptr(val));
  } else if constexpr (std::is_same_v<T, vec3>) {
    glUniform3fv(block_idx, 1, glm::value_ptr(val));
  } else if constexpr (std::is_same_v<T, vec4>) {
    glUniform4fv(block_idx, 1, glm::value_ptr(val));
  } else if constexpr (std::is_same_v<T, mat2>) {
    glUniformMatrix2fv(block_idx, 1, GL_FALSE, glm::value_ptr(val));
  } else if constexpr (std::is_same_v<T, mat3>) {
    glUniformMatrix3fv(block_idx, 1, GL_FALSE, glm::value_ptr(val));
  } else if constexpr (std::is_same_v<T, mat4>) {
    glUniformMatrix4fv(block_idx, 1, GL_FALSE, glm::value_ptr(val));
  } else {
    CORE_ERROR("{} set_uniform invaild", name);
  }
}

#define INSTANTIATE_TEMPLATE(T) \
  template void Shader::set_uniform(const GLchar *name, const T &val);

using namespace glm;

INSTANTIATE_TEMPLATE(int)
INSTANTIATE_TEMPLATE(bool)
INSTANTIATE_TEMPLATE(GLfloat)
INSTANTIATE_TEMPLATE(GLuint)
INSTANTIATE_TEMPLATE(vec2)
INSTANTIATE_TEMPLATE(vec3)
INSTANTIATE_TEMPLATE(vec4)
INSTANTIATE_TEMPLATE(mat2)
INSTANTIATE_TEMPLATE(mat3)
INSTANTIATE_TEMPLATE(mat4)

#undef INSTANTIATE_TEMPLATE

}  // namespace asset
