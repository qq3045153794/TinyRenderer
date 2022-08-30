#include "asset/Shader.h"
#include "asset/Texture.h"
#include "component/Camera.h"
#include "component/Mesh.h"
#include "component/Transform.h"
#include "core/Clock.h"
#include "core/Event.h"
#include "core/Window.h"
int main() {
  Window::instand().init();
  Input::init();
  Event::register_callback();

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  std::unique_ptr<Mesh> obj = std::make_unique<Mesh>(Mesh::QUAD);
  std::unique_ptr<component::CameraFps> main_camera = std::make_unique<component::CameraFps>(
      0.f, 1.f, 0.f, 1.f, 0.1f, 100.f);
  main_camera->set_position(glm::vec3(0.0, 0.0, 1.0));
  bool run = true;

  std::unique_ptr<asset::Shader> shader =
      std::make_unique<asset::Shader>("../examples/demo7/image.vs", "../examples/demo7/image.fs");
  std::unique_ptr<asset::Texture> tex =
      std::make_unique<asset::Texture>("../resource/texture/awesomeface.png");
  std::unique_ptr<component::Transform> T = std::make_unique<component::Transform>();

  while (run) {
    Window::instand().clear_buffer();
    shader->bind();
    main_camera->update();
    Clock::update();
    shader->set_uniform("projection", main_camera->get_projection_mat());
    const auto& view_mat = main_camera->get_view_mat();
    shader->set_uniform("view", view_mat);
    shader->set_uniform("model", T->get_transform());

    tex->bind(0);
    obj->draw();
    Window::instand().update();
  }
}