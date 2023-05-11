// clang-format off
#include <system/RenderSystem.h>
#include <scene/Scene.h>
#include <scene/RenderCommand.h>
#include <core/Window.h>
#include <library/FBOLibrary.h>
#include <library/ShaderLibrary.h>
#include <manage/ConfigManage.h>
// clang-format on
namespace saber {
namespace system {

using Texture = asset::Texture;
using FBO = ::asset::FBO;
using VAO = ::asset::VAO;
using VBO = ::asset::VBO;
using RenderCommand = ::scene::RenderCommand;
using Entity = ::scene::Entity;

RenderSystem::RenderSystem(scene::Scene* scene) : System(scene) {}

void RenderSystem::Awake() {}

void RenderSystem::OnUpdateRuntime() {
  // TODO
}

void RenderSystem::OnEditorRumtime(scene::Entity& editor_camera) {
  ::scene::RenderCommand::eable_depth_test(true);
  ::scene::RenderCommand::eable_alpha_blend(true);
  ::scene::RenderCommand::eable_face_culling(true);
  using namespace ::component;
  auto& reg = m_scene->registry;
  auto mesh_group = reg.group<Mesh>(entt::get<Transform, Material, Tag>);
  auto model_group = reg.group<Model>(entt::get<Transform, Tag>);
  auto& render_queue = m_scene->render_queue;

  /*
  float near_plane = 1.0f, far_plane = 7.5f;
  glm::vec3 lightPos(0.0f, 0.0f, 5.0f);
  auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  auto lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
  auto lightSpaceMatrix = lightProjection * lightView;
  */

  /*--------------------------------shadow Render Pass---------------------------------------*/
  auto shadow_fbo = PublicSingleton<Library<::asset::FBO>>::GetInstance().GetShadowFBO();
  auto shadow_shader = PublicSingleton<Library<::asset::Shader>>::GetInstance().GetShadowShader();
  auto shadow_texture = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("shadow_map");
  float near_plane = 1.F, far_plane = 50.F;
  auto dl_view = m_scene->registry.view<DirectionLight, Transform>();
    shadow_fbo->bind();
    ::scene::RenderCommand::clear_buffer();

  if (PublicSingleton<ConfigManage>::GetInstance().enable_shadow) {
    for (auto& e : dl_view) {
      auto ubo = PublicSingleton<Library<::asset::UBO>>::GetInstance().Get("DL");
      auto& dl = dl_view.get<DirectionLight>(e);
      auto& dt = dl_view.get<Transform>(e);
      auto lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
      auto lightView = dt.get_lookat();
      auto lightSpaceMatrix = lightProjection * lightView;

      for (auto& e : render_queue) {
        if (mesh_group.contains(e)) {
          auto& mesh = mesh_group.get<Mesh>(e);
          auto& transform = mesh_group.get<Transform>(e);
          auto& material = mesh_group.get<Material>(e);
          auto& tag = mesh_group.get<Tag>(e);
          material.bind();
          material.set_uniform("lightSpaceMatrix", lightSpaceMatrix);
          if (!tag.contains(ETag::Skybox)) {
            shadow_shader->bind();
            shadow_shader->set_uniform("model", transform.get_transform());
            shadow_shader->set_uniform("light_space_matrix", lightSpaceMatrix);
            auto is_animation_data = material.get_uniform("is_animation");
            auto is_animation = std::get<UboData<bool>>(is_animation_data).m_data;
            shadow_shader->set_uniform("is_animation", is_animation);
            if (is_animation) {
              auto bone_transforms = material.get_uniform("bone_transform");
              auto data = std::get<UboData<glm::mat4>>(bone_transforms);
              const std::vector<glm::mat4>* array_ptr = data.m_array_ptr;
              std::size_t array_length = (*array_ptr).size();
              for (std::size_t i = 0U; i < array_length; i++) {
                std::string label = "bone_transform[" + std::to_string(i) + "]";
                shadow_shader->set_uniform(label.c_str(), (*array_ptr)[i]);
              }
            }

            ::scene::RenderCommand::set_front_is_ccw(false);
            mesh.draw();

            ::scene::RenderCommand::set_front_is_ccw(true);
          } else {
            // 天空盒不渲染
            // ::scene::RenderCommand::set_front_is_ccw(false);
            // mesh.draw();
            // ::scene::RenderCommand::set_front_is_ccw(true);
          }
        } else if (model_group.contains(e)) {
          auto& model = model_group.get<Model>(e);
          auto& transform = model_group.get<Transform>(e);
          for (auto& mesh : model.meshes) {
            auto& material = model.materials.at(mesh.material_id);
            material.bind();
            material.set_uniform("lightSpaceMatrix", lightSpaceMatrix);
            shadow_shader->bind();
            shadow_shader->set_uniform("model", transform.get_transform());
            shadow_shader->set_uniform("light_space_matrix", lightSpaceMatrix);
            auto is_animation_data = material.get_uniform("is_animation");
            auto is_animation = std::get<UboData<bool>>(is_animation_data).m_data;
            shadow_shader->set_uniform("is_animation", is_animation);
            if (is_animation) {
              auto bone_transforms = material.get_uniform("bone_transform");
              auto data = std::get<UboData<glm::mat4>>(bone_transforms);
              const std::vector<glm::mat4>* array_ptr = data.m_array_ptr;
              std::size_t array_length = (*array_ptr).size();
              for (std::size_t i = 0U; i < array_length; i++) {
                std::string label = "bone_transform[" + std::to_string(i) + "]";
                shadow_shader->set_uniform(label.c_str(), (*array_ptr)[i]);
              }
            }
            shadow_shader->bind();
            shadow_shader->set_uniform("model", transform.get_transform());
            shadow_shader->set_uniform("light_space_matrix", lightSpaceMatrix);
            mesh.draw();
          }
        }
      }
      // 只有一处平行光
      break;
    }
  }
  shadow_fbo->ubind();

  /*--------------------------------Shadow Debug Render Pass---------------------------------*/
  /*
  auto shadow_debug_fbo = PublicSingleton<Library<::asset::FBO>>::GetInstance().GetDeBugShadowFBO();
  shadow_debug_fbo->bind();
  ::scene::RenderCommand::clear_buffer();
  auto shadow_debug_shader = PublicSingleton<Library<::asset::Shader>>::GetInstance().Get("Shadow Debug");
  shadow_debug_shader->bind();
  shadow_debug_shader->set_uniform("near_plane", near_plane);
  shadow_debug_shader->set_uniform("far_plane", far_plane);
  shadow_debug_shader->set_uniform("depthMap", 0);
  shadow_texture->bind(0);
  shadow_fbo->draw_ex();
  shadow_debug_shader->ubind();
  shadow_debug_fbo->ubind();
  */

  /*--------------------------------Render Pass----------------------------------------------*/
  auto main_fbo = PublicSingleton<Library<::asset::FBO>>::GetInstance().GetMainFBO();
  main_fbo->bind();
  ::scene::RenderCommand::clear_buffer();
  for (auto& e : render_queue) {
    if (mesh_group.contains(e)) {
      auto& mesh = mesh_group.get<Mesh>(e);
      auto& transform = mesh_group.get<Transform>(e);
      auto& material = mesh_group.get<Material>(e);
      auto& tag = mesh_group.get<Tag>(e);
      material.bind();
      material.set_uniform("model", transform.get_transform());
      if (!tag.contains(ETag::Skybox)) {
        mesh.draw();
      } else {
        ::scene::RenderCommand::set_front_is_ccw(false);
        mesh.draw();
        ::scene::RenderCommand::set_front_is_ccw(true);
      }
    } else if (model_group.contains(e)) {
      auto& model = model_group.get<Model>(e);
      auto& transform = model_group.get<Transform>(e);
      for (auto& mesh : model.meshes) {
        auto& material = model.materials.at(mesh.material_id);
        material.bind();
        material.set_uniform("model", transform.get_transform());
        mesh.draw();
      }
    }
  }
  main_fbo->ubind();
}

}  // namespace system
}  // namespace saber
