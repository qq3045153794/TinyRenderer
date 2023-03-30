#include <scene/SerializeEntity.h>
#include <utils/File.h>

#include <scene/Entity.hpp>
namespace scene {

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat2& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq;
  for (std::size_t i = 0; i < 2; i++) {
    for (std::size_t j = 0; j < 2; j++) {
      out << v[i][j];
    }
  }
  out << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat3& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq;
  for (std::size_t i = 0; i < 3; i++) {
    for (std::size_t j = 0; j < 3; j++) {
      out << v[i][j];
    }
  }
  out << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& v) {
  out << YAML::Flow;
  out << YAML::BeginSeq;
  for (std::size_t i = 0; i < 4; i++) {
    for (std::size_t j = 0; j < 4; j++) {
      out << v[i][j];
    }
  }
  out << YAML::EndSeq;
  return out;
}

void SerializeObject::SerializeEntity(const std::filesystem::path& file_name_path, Entity& entity) {
  YAML::Emitter out;
  out << YAML::BeginMap;

  std::cout << "file_name_path : " << file_name_path.string() << std::endl;

  auto& tag_component = entity.GetComponent<::component::Tag>();
  out << YAML::Key << "TagComponent";
  out << YAML::BeginMap;
  out << YAML::Key << "Tag" << YAML::Value << static_cast<int>(tag_component.m_tag);
  out << YAML::Key << "EntityName" << YAML::Value << tag_component.m_name;
  out << YAML::EndMap;

  auto& transform_component = entity.GetComponent<::component::Transform>();
  out << YAML::Key << "TransformComponent";
  out << YAML::BeginMap;
  out << YAML::Key << "Position" << YAML::Value << transform_component.get_position();
  out << YAML::Key << "Rotation" << YAML::Value << transform_component.get_eular();
  out << YAML::Key << "Scale" << YAML::Value << transform_component.get_scale();
  out << YAML::EndMap;

  if (entity.Contains<::component::Mesh>()) {
    auto& mesh_component = entity.GetComponent<::component::Mesh>();
    out << YAML::Key << "MeshComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "primitive" << YAML::Value << static_cast<int>(mesh_component.m_obj);
    out << YAML::EndMap;
  }

  if (entity.Contains<::component::Material>()) {
    auto& material_component = entity.GetComponent<::component::Material>();
    out << YAML::Key << "MaterialComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "ShadingModel" << YAML::Value << static_cast<uint16_t>(material_component.m_shading_model);
    std::array<std::string, 10> texture_dictionary = {
        "texture_0",      "texture_1",         "irradiance_map",    "prefilter_map", "brdf_LUT_map",
        "albedo_texture", "metalness_texture", "roughness_texture", "ao_texture",    "normal_texture"};
    for (auto& [u_id, texture] : material_component.m_textures) {
      if (texture->m_image_path) {
        out << YAML::Key << texture_dictionary[u_id] + "_path" << YAML::Value << texture->m_image_path->string();
      }
    }

    for (auto& [name_key, value] : material_component.m_uniforms_cache) {
      std::visit([&out](auto& val) { out << YAML::Key << val.m_name << YAML::Value << val.m_data; }, value);
    }

    // if (material_component.m_shading_model == ::component::Material::ShadingModel::DEFAULT) {
    //} else if (material_component.m_shading_model == ::component::Material::ShadingModel::PBR) {
    //} else {
    // CORE_ERROR("Unexpection...");
    //}

    out << YAML::EndMap;
  }

  if (entity.Contains<::component::Camera>()) {
    auto& camera_component = entity.GetComponent<::component::Camera>();
    out << YAML::Key << "CameraComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Projection" << YAML::Value << camera_component.m_camera_projection;
    out << YAML::Key << "Fov" << YAML::Value << camera_component.m_fov;
    out << YAML::Key << "Aspect" << YAML::Value << camera_component.m_aspect;
    out << YAML::Key << "Left" << YAML::Value << camera_component.m_left;
    out << YAML::Key << "Right" << YAML::Value << camera_component.m_right;
    out << YAML::Key << "Bottom" << YAML::Value << camera_component.m_bottom;
    out << YAML::Key << "Top" << YAML::Value << camera_component.m_top;
    out << YAML::Key << "Znear" << YAML::Value << camera_component.m_znear;
    out << YAML::Key << "Zfar" << YAML::Value << camera_component.m_zfar;
    out << YAML::EndMap;
  }

  if (entity.Contains<::component::CameraFps>()) {
    auto& camerafps_component = entity.GetComponent<::component::CameraFps>();
    out << YAML::Key << "CameraFPSComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Projection" << YAML::Value << camerafps_component.m_camera_projection;
    out << YAML::Key << "Fov" << YAML::Value << camerafps_component.m_fov;
    out << YAML::Key << "Aspect" << YAML::Value << camerafps_component.m_aspect;
    out << YAML::Key << "Left" << YAML::Value << camerafps_component.m_left;
    out << YAML::Key << "Right" << YAML::Value << camerafps_component.m_right;
    out << YAML::Key << "Bottom" << YAML::Value << camerafps_component.m_bottom;
    out << YAML::Key << "Top" << YAML::Value << camerafps_component.m_top;
    out << YAML::Key << "Znear" << YAML::Value << camerafps_component.m_znear;
    out << YAML::Key << "Zfar" << YAML::Value << camerafps_component.m_zfar;
    out << YAML::EndMap;
  }

  if (entity.Contains<::component::DirectionLight>()) {
    auto& direction_light = entity.GetComponent<::component::DirectionLight>();
    out << YAML::Key << "DirectionLightComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Color" << YAML::Value << direction_light.m_color;
    out << YAML::Key << "Intensity" << YAML::Value << direction_light.m_intensity;
    out << YAML::EndMap;
  }

  if (entity.Contains<::component::PointLight>()) {
    auto& point_light = entity.GetComponent<::component::PointLight>();
    out << YAML::Key << "PointLightComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Color" << YAML::Value << point_light.m_color;
    out << YAML::Key << "Intensity" << YAML::Value << point_light.m_intensity;
    out << YAML::Key << "Linear" << YAML::Value << point_light.m_linear;
    out << YAML::Key << "Quadratic" << YAML::Value << point_light.m_quadratic;
    out << YAML::EndMap;
  }

  if (entity.Contains<::component::Spotlight>()) {
    auto& spot_light = entity.GetComponent<::component::Spotlight>();
    out << YAML::Key << "SpotLightComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "Color" << YAML::Value << spot_light.m_color;
    out << YAML::Key << "Intensity" << YAML::Value << spot_light.m_intensity;
    out << YAML::Key << "Range" << YAML::Value << spot_light.m_range;
    out << YAML::Key << "Inner_cutoff" << YAML::Value << spot_light.m_intensity;
    out << YAML::Key << "Outer_cutoff" << YAML::Value << spot_light.m_outer_cutoff;
    out << YAML::EndMap;
  }
  out << YAML::EndMap;

  ::utils::File::write_yml_file(file_name_path, out);
}

}  // namespace scene
