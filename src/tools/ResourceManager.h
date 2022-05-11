#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include "glad/glad.h"
#include "Shader.h"
#include "Texture2D.h"

#include <map>
#include <string>
#include <sstream>
#include <fstream>

class ResourceManager
{
public:
  //资源存储
  static std::map<std::string, Shader> shaders;
  static std::map<std::string, Texture2D> texture2Ds;
  //从文件加载着色器
  static Shader load_shader(const GLchar *vshader_file, const GLchar *fshader_file, const GLchar *gshader_file, std::string name);
  static Shader load_shader(const GLchar *vshader_file, const GLchar *fshader_file, std::string name);
  //检索存储的的着色器
  static Shader &get_shader(std::string name);
  //从文件中加载2D纹理
  static Texture2D load_texture(const GLchar *file, std::string name);
  //检索存储的的2D纹理
  static Texture2D &get_texture(std::string name);
  //清空
  static void clear();

private:
  ResourceManager() {}
  static Shader load_shader_from_file(const GLchar *vshader_file, const GLchar *fshader_file);
  static Shader load_shader_from_file(const GLchar *vshader_file, const GLchar *fshader_file, const GLchar *gshader_file);
  static Texture2D load_texture_from_file(const GLchar *file);
};
#endif