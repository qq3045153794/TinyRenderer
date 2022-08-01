#include "core/App.h"

#include <iostream>

#include "component/Mesh.h"
#include "core/Window.h"
bool App::m_app_run = true;

App& App::instand() {
  static App i;
  return i;
}

void App::init() {
  Window::instand().init();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return;
  }

  m_mesh = std::make_shared<Mesh>(Mesh::QUAD);

  m_shader =
      std::make_shared<asset::Shader>("../examples/demo6/image.vs", "../examples/demo6/image.fs");

  m_texture = std::make_shared<asset::Texture>("../resource/texture/awesomeface.png");


  glm::mat4 model(1.0), view(1.0);
  glm::mat4 projection = glm::ortho(0.0f, (GLfloat)Window::instand().m_width, 0.0f,
                                    (GLfloat)Window::instand().m_height, -1.0f, 1.0f);
  model = glm::scale(model, glm::vec3(Window::instand().m_width, Window::instand().m_height, 1.0));
  m_shader->bind();
  m_shader->set_uniform("projection", projection);
  m_shader->set_uniform("view", view);
  m_shader->set_uniform("model", model);
  m_shader->set_uniform("texture_1", 0);

}

void App::run() {
  while (m_app_run) {
    Window::instand().clear_buffer();
    m_shader->bind();
    m_texture->bind(0);
    m_mesh->draw();
    Window::instand().update();
  }
}

App::App() {}

App::~App() { Window::instand().clear(); }
