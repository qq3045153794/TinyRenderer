
#include "Image.h"

Image::Image() {
    setup_buffer();
}


Image::~Image() {
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

void Image::setup_buffer() {
    GLfloat data[6][5] = {
        {0.0, 0.0, 0.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 0.0, 0.0},
        {1.0, 0.0, 0.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0, 1.0},
        {0.0, 1.0, 0.0, 0.0, 0.0}, {1.0, 1.0, 1.0, 1.0, 1.0}
    };

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 5, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)(sizeof(GLfloat) *3));
    glEnableVertexAttribArray(1);
}

std::shared_ptr<Image> create_image_model() {
    return std::shared_ptr<Image> (new Image());
}

void Image::draw(Shader& shader) {
    shader.use();
    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 scale_model(1.0);
    glm::mat4 translate_model(1.0);
    glm::mat4 quat_model(1.0);

    scale_model = glm::scale(scale_model, glm::vec3(m_sz, 1.0));
    translate_model = glm::translate(translate_model, m_position);   
    translate_model = glm::translate(translate_model, glm::vec3(-m_sz.x * 0.5, -m_sz.y * 0.5, 0.0));
    quat_model = glm::mat4_cast(m_rotation);

    model = quat_model * translate_model * scale_model;

    shader.set_matrix4("model", model);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}