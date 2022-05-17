#include "RenderText.h"

RenderText::RenderText(Shader shader):
    shader(shader) {
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void RenderText::draw(wchar_t* text, GLfloat x, GLfloat y, glm::vec3 color) {
    FT_Library ft;
    FT_Face face;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    
    if (FT_New_Face(ft, "../resource/fronts/song.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 24);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    shader.use();
    shader.set_vector3f("textColor", glm::vec3(color.x, color.y, color.z));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->vao);
    for(GLuint i = 0, sz = wcslen(text); i < sz; i++) {
        if(FT_Load_Char(face, text[i], FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character ch = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        GLfloat x_pos = x + ch.bearing.x;
        GLfloat y_pos = y + ch.bearing.y - ch.size.y;
        GLfloat w = ch.size.x;
        GLfloat h = ch.size.y;
        GLfloat vertices[6][4] = {
            { x_pos,     y_pos + h,   0.0, 0.0 },            
            { x_pos,     y_pos,       0.0, 1.0 },
            { x_pos + w, y_pos,       1.0, 1.0 },

            { x_pos,     y_pos + h,   0.0, 0.0 },
            { x_pos + w, y_pos,       1.0, 1.0 },
            { x_pos + w, y_pos + h,   1.0, 0.0 }           
        };
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6);
        glDeleteTextures(1, &ch.texture_id);
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}