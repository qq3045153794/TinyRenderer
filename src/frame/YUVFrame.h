#ifndef _LEARN_OPENGL_SRC_FRAME_YUVFRAME_H_
#define _LEARN_OPENGL_SRC_FRAME_YUVFRAME_H_
#include "ResourceManager.h"
namespace gl_frame {

class YUVFrame {
 public:
    YUVFrame(GLuint width, GLuint height);
    YUVFrame(const YUVFrame&) = delete;
    const YUVFrame& operator=(const YUVFrame&) = delete;
    ~YUVFrame();
    void begin_render();
    void end_render();
    void render();
 private:
    void setup_frame_buffer();
    void setup_screem_buffer();
    Shader m_shader;
    Texture2D m_texture;
    GLuint m_width;
    GLuint m_height;
    // 帧缓冲
    GLuint m_fbo;
    // 渲染缓冲
    GLuint m_rbo;
    GLuint m_vao;
    GLuint m_vbo;
};

}
#endif