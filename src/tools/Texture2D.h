#include "glad/glad.h"
#include "GLFW/glfw3.h"

#ifndef _TEXTURE_2D_H_
#define _TEXTURE_2D_H_
class Texture2D {
public:
  Texture2D();
  void set_image(GLuint width, GLuint height, unsigned char* data);
  void bind() const;

  void set_format(GLuint internal_format, GLuint image_format);
  void set_wrap(GLuint warp_s, GLuint warp_t);
  void set_filter(GLuint filter_min, GLuint filter_max);
private:
  GLuint texture;           //纹理buffer
  GLuint width;             //图片宽
  GLuint height;            //图片高
  /*纹理和图片格式*/
  GLuint internal_format;   //纹理格式 rgb or rgba
  GLuint image_format;      //图片格式 rgb(jpg) or rgba(png)
  /*纹理环绕*/
  GLuint wrap_s;            //s(x)轴纹理环绕方式
  GLuint wrap_t;            //t(y)轴纹理环绕方式
  /*纹理过滤*/
  GLuint filter_min;        //缩小时纹理过滤方式
  GLuint filter_max;        //放大时纹理过滤方式
};
#endif