#ifndef _RENDER_TEXT_H_
#define _RENDER_TEXT_H_
#include "ResourceManager.h"
#include "ft2build.h"
#include FT_FREETYPE_H 

#include <wchar.h>
struct Character {
    GLuint texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};
class RenderText{
public:
    RenderText(Shader shader);
    void draw(wchar_t* text, GLfloat x, GLfloat y, glm::vec3 color);
private:
    GLuint vao;
    GLuint vbo;
    Shader shader;
};
#endif