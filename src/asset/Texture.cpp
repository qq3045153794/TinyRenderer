#include "asset/Texture.h" 
#include <iostream>
#include <memory>
#include "stb_image.h"
#include "utils/image.h"
#include "asset/FBO.h"
namespace asset {

Texture::Texture(const GLchar* img_path, bool flip) : m_target(GL_TEXTURE_2D) {
  const auto& image = utils::Image(img_path, flip);
  m_width = image.get_width();
  m_height = image.get_height();
  m_image_format = image.get_img_format();
  m_internal_format = image.get_ine_format();
  glGenTextures(1, &m_id);
  glBindTexture(m_target, m_id);
  glTexImage2D(m_target, 0, m_internal_format, m_width, m_height, 0, m_image_format,
               GL_UNSIGNED_BYTE, image.get_buffer());
  glBindTexture(m_target, 0);

  set_sampler_state();
}

Texture::Texture(const std::vector<std::string>& path_vec) : m_target(GL_TEXTURE_CUBE_MAP) {
  glGenTextures(1, &m_id);
  glBindTexture(m_target, m_id);

  int idx = 0;
  for (const auto& img_path : path_vec) {
    const auto& image = utils::Image(img_path);
    m_width = image.get_width();
    m_height = image.get_height();
    m_image_format = image.get_img_format();
    m_internal_format = image.get_ine_format();

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + idx, 0, m_internal_format, m_width, m_height, 0,
                 m_image_format, GL_UNSIGNED_BYTE, image.get_buffer());
    ++idx;
  }
  glBindTexture(m_target, 0);
  set_sampler_state();
}

Texture::Texture(const GLchar* path, int resolution) : m_target(GL_TEXTURE_CUBE_MAP){


  const auto& image = utils::Image(path, true);
  m_width = image.get_width();
  m_height = image.get_height();
  m_image_format = image.get_img_format();
  m_internal_format = image.get_ine_format();
  // 创建 hdr 2D texture
  GLuint hdr_id;
  glGenTextures(1, &hdr_id);
  glBindTexture(GL_TEXTURE_2D, hdr_id);
  glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format, m_width, m_height, 0, m_image_format,
               GL_FLOAT, image.get_buffer());
  glBindTexture(GL_TEXTURE_2D, 0);

  std::unique_ptr<FBO> cubemap_fbo = std::make_unique<FBO>(resolution, resolution);

  cubemap_fbo->set_depth_texture();

  const size_t faces = 6;

  // 创建cube
  glGenTextures(1, &m_id);
  glBindBuffer(m_target, m_id);
  for (size_t i = 0; i < faces; i++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 
                 resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);
  }

  set_sampler_state();

  glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.f);

  glm::mat4 views[] = 
  {
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
     glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
  };

 float data[] = {
    // back face
    -1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    // front face
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    // left face
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    // right face
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    // bottom face
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    // top face
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f , 1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
  };


  auto cube_vbo = std::make_unique<VBO>(sizeof(data), data, GL_STATIC_DRAW);

  auto cube_vao = std::make_unique<VAO>();

  cube_vao->set_vbo(*cube_vbo, 0U, 3U, 3U * sizeof(int), 0, GL_FLOAT);

  cubemap_fbo->change_shader(std::make_unique<Shader>("../TinyRenderer/resource/shader/hdr2cubemap", "../TinyRenderer/resource/shader/hdr2cubemap"));

  auto& cubemap_shader = cubemap_fbo->get_shader();

  cubemap_shader->bind();
  cubemap_shader->set_uniform("projection", proj);

  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, hdr_id);

  glViewport(0, 0, resolution, resolution);
  cubemap_fbo->bind();

  for (size_t i = 0; i < faces; i++) {
    cubemap_shader->set_uniform("view", views[i]);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_id, 0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cube_vao->bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);

    cube_vao->ubind();

  }
  
}
Texture::Texture(GLuint width, GLuint height) : m_target(GL_TEXTURE_2D) {
  m_width = width;
  m_height = height;
  m_image_format = GL_RGB;
  m_internal_format = GL_RGB;
  glGenTextures(1, &m_id);
  glBindTexture(m_target, m_id);
  glTexImage2D(m_target, 0, m_internal_format, m_width, m_height, 0, m_image_format,
               GL_UNSIGNED_BYTE, NULL);
  glBindTexture(m_target, 0);

  set_sampler_state();
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

GLuint Texture::get_id() { return m_id; }

void Texture::set_sampler_state() {
  glBindTexture(m_target, m_id);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (m_target == GL_TEXTURE_CUBE_MAP) {
    glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
  }

  glBindTexture(m_target, 0);
}

void Texture::bind(GLuint unit) const {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(m_target, m_id);
}

void Texture::ubind(GLuint unit) const { glBindTexture(m_target, 0); }

}  // namespace asset
