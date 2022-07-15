#ifndef _LEARN_OPENGL_SRC_FRAME_YUVFRAME_H_
#define _LEARN_OPENGL_SRC_FRAME_YUVFRAME_H_
#include "ResourceManager.h"
namespace gl_frame {

// MASS 抗锯齿 后期处理
class YUVFrame {
 public:
    YUVFrame(GLuint width, GLuint height, GLuint sampler);
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
    Texture2D m_mult_texture;
    GLuint m_width;
    GLuint m_height;
    GLuint m_sampler;
    // 帧缓冲
    GLuint m_fbo;
    GLuint m_mult_fbo;
    // 渲染缓冲
    GLuint m_mult_rbo;
    GLuint m_vao;
    GLuint m_vbo;
};

}
#endif