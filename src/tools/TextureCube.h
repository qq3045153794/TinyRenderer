#ifndef _LEARN_OPENGL_SRC_TOOLS_TEXTURECUBE_H_
#define _LEARN_OPENGL_SRC_TOOLS_TEXTURECUBE_H_

#include <iostream>
#include <vector>

#include "GLFW/glfw3.h"
#include "glad/glad.h"
class TextureCube {
 public:
  TextureCube();
  ~TextureCube(){};
  void set_image(std::vector<GLuint> width, std::vector<GLuint> height,
                 const std::vector<unsigned char*>& data);
  void Bind() const;
  void set_wrap(GLuint warp_s, GLuint warp_t, GLuint warp_r);
  void set_filter(GLuint filter_min, GLuint filter_max);
  void set_format(const std::vector<GLuint> image_format,
                  const std::vector<GLuint> internal_format);

 private:
  GLuint m_texture;              //纹理buffer
  std::vector<GLuint> m_width;   //图片宽
  std::vector<GLuint> m_height;  //图片高
  /*纹理和图片格式*/
  std::vector<GLuint> m_internal_format;  //纹理格式 rgb or rgba
  std::vector<GLuint> m_image_format;     //图片格式 rgb(jpg) or rgba(png)
  /*纹理环绕*/
  GLuint m_wrap_s;  // s(x)轴纹理环绕方式
  GLuint m_wrap_t;  // t(y)轴纹理环绕方式
  GLuint m_wrap_r;  // r(z)轴纹理环绕方式
  /*纹理过滤*/
  GLuint m_filter_min;  //缩小时纹理过滤方式
  GLuint m_filter_max;  //放大时纹理过滤方式
};

#endif