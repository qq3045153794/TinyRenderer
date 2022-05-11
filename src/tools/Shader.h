#ifndef _SHADER_H_
#define _SHADER_H_
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

class Shader
{
public:
    //着色器句柄
    GLuint ID; 
    
    Shader() { }
    //让状态机绑定当前着色器
    Shader& use();
    //设置着色器
    void    compile     (const GLchar* vertex_code, const GLchar* fragment_code, const GLchar *geometry_code); 
    void    compile     (const GLchar* vertex_code, const GLchar* fragment_code);
    // 设置uniform值
    void    set_float    (const GLchar* name, GLfloat value, GLboolean use_shader = false);
    void    set_integer  (const GLchar* name, GLint value, GLboolean use_shader = false);
    void    set_vector2f (const GLchar* name, GLfloat x, GLfloat y, GLboolean use_shader = false);
    void    set_vector2f (const GLchar* name, const glm::vec2 &value, GLboolean use_shader = false);
    void    set_vector3f (const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean use_shader = false);
    void    set_vector3f (const GLchar* name, const glm::vec3 &value, GLboolean use_shader = false);
    void    set_vector4f (const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean use_shader = false);
    void    set_vector4f (const GLchar* name, const glm::vec4 &value, GLboolean use_shader = false);
    void    set_matrix4  (const GLchar* name, const glm::mat4 &matrix, GLboolean use_shader = false);
private:
    GLuint  create_shader       (const GLchar* code,GLuint type);
    void    check_compile_errors(GLuint object, const std::string& type); 
};

#endif