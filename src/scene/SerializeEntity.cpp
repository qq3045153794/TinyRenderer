#include <library/TextureLibrary.h>
#include <scene/Scene.h>
#include <scene/SerializeEntity.h>

#include <scene/Entity.hpp>
namespace YAML {
template <>
struct convert<glm::vec2> {
  static Node encode(const glm::vec2& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.SetStyle(EmitterStyle::Flow);
    return node;
  }

  static bool decode(const Node& node, glm::vec2& rhs) {
    if (!node.IsSequence() || node.size() != 2) return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    return true;
  }
};

template <>
struct convert<glm::vec3> {
  static Node encode(const glm::vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  static bool decode(const Node& node, glm::vec3& rhs) {
    if (!node.IsSequence() || node.size() != 3) return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
  }
};

template <>
struct convert<glm::vec4> {
  static Node encode(const glm::vec4& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.push_back(rhs.w);
    return node;
  }

  static bool decode(const Node& node, glm::vec4& rhs) {
    if (!node.IsSequence() || node.size() != 4) return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    rhs.w = node[3].as<float>();
    return true;
  }
};
}  // namespace YAML
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

void SerializeObject::SerializeEntity(Entity& entity, YAML::Emitter& out) {
  out << YAML::BeginMap;
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
    if (material_component.m_shading_model == ::component::Material::ShadingModel::COSTEM) {
      out << YAML::Key << "VertexShaderPath" << YAML::Value
          << material_component.m_shader->m_vertex_file_name_path.string();
      out << YAML::Key << "FragmentShaderPath" << YAML::Value
          << material_component.m_shader->m_fragment_file_name_path.string();
    }
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

    out << YAML::EndMap;
  }

  if (entity.Contains<::component::Model>()) {
    auto& model_conponent = entity.GetComponent<::component::Model>();
    out << YAML::Key << "ModelComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "ModelFliePath" << YAML::Value << model_conponent.model_filepath;
    out << YAML::Key << "isAnimation" << YAML::Value << model_conponent.m_animated;
    out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
    for (auto& [texture_name, uid] : model_conponent.materials_cache) {
      auto& material = model_conponent.materials.at(uid);
      out << YAML::BeginMap;
      out << YAML::Key << "MaterialName" << YAML::Value << texture_name;
      out << YAML::Key << "ShadingModel" << YAML::Value << static_cast<uint16_t>(material.m_shading_model);
      std::array<std::string, 10> texture_dictionary = {
          "texture_0",      "texture_1",         "irradiance_map",    "prefilter_map", "brdf_LUT_map",
          "albedo_texture", "metalness_texture", "roughness_texture", "ao_texture",    "normal_texture"};
      for (auto& [u_id, texture] : material.m_textures) {
        if (texture->m_image_path) {
          out << YAML::Key << texture_dictionary[u_id] + "_path" << YAML::Value << texture->m_image_path->string();
        }
      }

      for (auto& [name_key, value] : material.m_uniforms_cache) {
        std::visit(
            [&out](auto& val) {
              // bone transform 不需要序列化 特判下
              if (val.m_name != "bone_transform") {
                out << YAML::Key << val.m_name << YAML::Value << val.m_data;
              }
            },
            value);
      }
      out << YAML::EndMap;
    }
    out << YAML::EndSeq;

    if (model_conponent.m_animated) {
      out << YAML::Key << "AnimationPathfile" << YAML::Value << model_conponent.animation_filepath;
    }
    out << YAML::EndMap;
  }

  if (entity.Contains<::component::Animator>()) {
    out << YAML::Key << "AnimatorComponent";
    out << YAML::BeginMap;
    out << YAML::Key << "node" << YAML::Value << "node";
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
}

void SerializeObject::SerializeEntity(const std::filesystem::path& file_name_path, Entity& entity) {
  YAML::Emitter out;
  SerializeEntity(entity, out);
  ::utils::File::write_yml_file(file_name_path, out);
}

Entity SerializeObject::DeserializeEntity(const std::filesystem::path& file_name_path, Scene& scene) {
  YAML::Node doc;
  ::utils::File::parser_yml_file(file_name_path.c_str(), doc);
  return DeserializeEntity(scene, doc);
}

void SerializeObject::SerializeScene(const std::filesystem::path& file_name_path, Scene& scene) {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "SceneName" << YAML::Value << scene.m_title;
  out << YAML::Key << "Entitys" << YAML::Value << YAML::BeginSeq;
  scene.registry.each([&](auto& entity_id) {
    Entity e{"temp", entity_id, &scene.registry};
    if (!scene.is_exclude_entity(entity_id)) {
      SerializeEntity(e, out);
    }
  });
  out << YAML::EndSeq;
  out << YAML::EndMap;
  ::utils::File::write_yml_file(file_name_path, out);
}

void SerializeObject::DeserializeScene(const std::filesystem::path& file_name_path, Scene& scene) {
  YAML::Node doc;
  ::utils::File::parser_yml_file(file_name_path.c_str(), doc);
  auto scene_name = doc["SceneName"].as<std::string>();
  scene.m_title = scene_name;
  YAML::Node entitys = doc["Entitys"];
  if (entitys) {
    for (int i = 0; i < entitys.size(); i++) {
      auto entity = DeserializeEntity(scene, entitys[i]);
      auto& tag = entity.GetComponent<::component::Tag>();
      if (tag.m_tag == ::component::ETag::MainCamera || tag.m_tag == ::component::ETag::Lamplight) {
        CORE_DEBUG("It is not render");
      } else {
        scene.SubmitRender(entity.id);
      }
    }
  }
}

Entity SerializeObject::DeserializeEntity(Scene& scene, const YAML::Node& doc) {
  auto tag_component = doc["TagComponent"];
  CORE_ASERT(tag_component, "tag_component is no exits");
  auto tag = tag_component["Tag"].as<int>();
  auto entity_name = tag_component["EntityName"].as<std::string>();
  auto entity = scene.create_entity(entity_name, static_cast<::component::ETag>(tag));

  auto transform_component = doc["TransformComponent"];
  CORE_ASERT(transform_component, "transform_component is no exits");
  auto position = transform_component["Position"].as<glm::vec3>();
  auto rotation = transform_component["Rotation"].as<glm::vec3>();
  auto scale = transform_component["Scale"].as<glm::vec3>();
  auto& transform = entity.GetComponent<::component::Transform>();
  transform.set_position(position);
  transform.set_ealar_YZX(rotation);
  transform.set_scale(scale);

  auto mesh_component = doc["MeshComponent"];
  if (mesh_component) {
    auto primitive = mesh_component["primitive"].as<int>();
    entity.AddComponent<::component::Mesh>(static_cast<::component::Mesh::primitive>(primitive));
  }

  auto material_component = doc["MaterialComponent"];
  if (material_component) {
    std::array<std::string, 10> texture_dictionary = {
        "texture_0",      "texture_1",         "irradiance_map",    "prefilter_map", "brdf_LUT_map",
        "albedo_texture", "metalness_texture", "roughness_texture", "ao_texture",    "normal_texture"};

    auto shading_model = material_component["ShadingModel"].as<uint16_t>();
    if (static_cast<::component::Material::ShadingModel>(shading_model) ==
        ::component::Material::ShadingModel::DEFAULT) {
      auto& material =
          entity.AddComponent<::component::Material>(static_cast<::component::Material::ShadingModel>(shading_model));
      for (std::size_t i = 0; i < texture_dictionary.size(); i++) {
        auto texture_key = texture_dictionary[i];
        auto texture_path_doc = material_component[texture_key + "_path"];
        if (texture_path_doc) {
          auto texture_path = texture_path_doc.as<std::string>();
          material.set_texture(i, std::make_shared<::asset::Texture>(texture_path.c_str(), true, 7));
        }
      }
    } else if (static_cast<::component::Material::ShadingModel>(shading_model) ==
               ::component::Material::ShadingModel::PBR) {
      auto& material =
          entity.AddComponent<::component::Material>(static_cast<::component::Material::ShadingModel>(shading_model));
      auto brdf_lut_texture =
          ::saber::PublicSingleton<::saber::Library<::asset::Texture>>::GetInstance().Get("BRDF_LUT");
      auto irradian_texture =
          ::saber::PublicSingleton<::saber::Library<::asset::Texture>>::GetInstance().Get("irradian");
      auto prefiltermap =
          ::saber::PublicSingleton<::saber::Library<::asset::Texture>>::GetInstance().Get("prefiltermap");
      material.bind_texture(::component::Material::pbr_t::irradiance_map, irradian_texture);
      material.bind_texture(::component::Material::pbr_t::prefilter_map, prefiltermap);
      material.bind_texture(::component::Material::pbr_t::brdf_LUT_map, brdf_lut_texture);
      std::array<std::string, 5> uniform_dictionary = {"albedo", "metalness", "roughness", "ao", "specular"};
      // 设置pbr的数值值 若有texture 将显示texture 则这步设置被覆盖 设置无效
      for (std::size_t i = 0; i < uniform_dictionary.size(); i++) {
        if (uniform_dictionary[i] == "albedo") {
          auto albedo = material_component[uniform_dictionary[i]].as<glm::vec4>();
          material.bind_uniform(static_cast<::component::Material::pbr_u>(i), albedo);
        } else {
          auto float_val = material_component[uniform_dictionary[i]].as<float>();
          material.bind_uniform(static_cast<::component::Material::pbr_u>(i), float_val);
        }
      }

      for (std::size_t i = 0; i < texture_dictionary.size(); i++) {
        auto texture_key = texture_dictionary[i];
        auto texture_path_doc = material_component[texture_key + "_path"];
        if (texture_path_doc) {
          auto texture_path = texture_path_doc.as<std::string>();
          if (i >= 5U) {
            material.bind_texture(static_cast<::component::Material::pbr_t>(i),
                                  std::make_shared<::asset::Texture>(texture_path.c_str(), true, 7));
            CORE_DEBUG("pbr path : {}", texture_path);
          }
        }
      }
    } else if (static_cast<::component::Material::ShadingModel>(shading_model) ==
               ::component::Material::ShadingModel::COSTEM) {
      // 目前自定义材质不完善 等后续修改
      auto vertex_shader_path = material_component["VertexShaderPath"].as<std::string>();
      auto fragment_shader_path = material_component["FragmentShaderPath"].as<std::string>();

      auto& material = entity.AddComponent<::component::Material>(
          std::make_shared<::asset::Shader>(vertex_shader_path.c_str(), fragment_shader_path.c_str()));
      for (std::size_t i = 0; i < texture_dictionary.size(); i++) {
        auto texture_key = texture_dictionary[i];
        auto texture_path_doc = material_component[texture_key + "_path"];
        if (texture_path_doc) {
          auto texture_path = texture_path_doc.as<std::string>();
          material.set_texture(i, std::make_shared<::asset::Texture>(texture_path.c_str(), true, 7));
        }
      }
    } else {
      CORE_WARN("Not to do...");
    }
  }

  auto model_component = doc["ModelComponent"];
  if (model_component) {
    auto model_filepath = model_component["ModelFliePath"].as<std::string>();
    auto is_animation = model_component["isAnimation"].as<bool>();
    auto& model = entity.AddComponent<::component::Model>(model_filepath, ::component::Auto, is_animation);
    auto materials = model_component["Materials"];

    // CORE_ASERT(materials.size() == model.materials_cache.size(), "Unexpection material count (count : {})...", mater)
    // int material_indence = 0;

    // 不存在有Animator但没有Model的状态
    // 所以Animator 的反序列化在Model反序了化时候完成 
    if (is_animation) {
      CORE_ASERT(model_component["AnimationPathfile"], "If there is animation, there must be AnimationPathfile");
      auto animation_filepath = model_component["AnimationPathfile"].as<std::string>();
      model.AttachMotion(animation_filepath);
      CORE_ASERT(doc["AnimatorComponent"], "If the model is animated, it must be bound to Animotor");
      entity.AddComponent<::component::Animator>(&model);
    }

    std::array<std::string, 10> texture_dictionary = {
        "texture_0",      "texture_1",         "irradiance_map",    "prefilter_map", "brdf_LUT_map",
        "albedo_texture", "metalness_texture", "roughness_texture", "ao_texture",    "normal_texture"};
    for (std::size_t i = 0U; i < materials.size(); i++) {
      auto material_doc = materials[i];
      auto Material_name = material_doc["MaterialName"].as<std::string>();
      auto shading_model = material_doc["ShadingModel"].as<uint16_t>();
      if (static_cast<::component::Material::ShadingModel>(shading_model) ==
          ::component::Material::ShadingModel::DEFAULT) {
        auto oringe_material = std::make_shared<::component::Material>(::component::Material::ShadingModel::DEFAULT);
        auto& material = model.SetMatermial(Material_name, *oringe_material);

        for (std::size_t i = 0; i < texture_dictionary.size(); i++) {
          auto texture_key = texture_dictionary[i];
          auto texture_path_doc = material_doc[texture_key + "_path"];
          if (texture_path_doc) {
            auto texture_path = texture_path_doc.as<std::string>();
            // 目前纹理序列化还不完善 先写死
            material.set_texture(i, std::make_shared<::asset::Texture>(texture_path.c_str(), false, 7));
          }
        }

        if (is_animation) {
          CORE_ASERT(doc["AnimatorComponent"], "If the model is animated, it must be bound to Animotor");
          auto& animator = entity.GetComponent<::component::Animator>();
          auto& bone_transforms = animator.m_bone_transforms;
          material.set_bound_arrary("bone_transform", 0U, &bone_transforms);
        }
      } else if (static_cast<::component::Material::ShadingModel>(shading_model) ==
                 ::component::Material::ShadingModel::PBR) {
        // TODO
      }
    }
    /*
    for (auto& [texture_name, uid] : model.materials_cache) {
      auto material_path = model_component[]
      auto& material = model.materials.at(uid);
      material.set_texture(0U, );
    }
    */
  }

  auto camera_component = doc["CameraComponent"];
  if (camera_component) {
    auto Projection = camera_component["Projection"].as<int>();
    auto fov = camera_component["Fov"].as<float>();
    auto aspect = camera_component["Aspect"].as<float>();
    auto left = camera_component["Left"].as<float>();
    auto right = camera_component["Right"].as<float>();
    auto bottom = camera_component["Bottom"].as<float>();
    auto top = camera_component["Top"].as<float>();
    auto z_near = camera_component["Znear"].as<float>();
    auto z_far = camera_component["Zfar"].as<float>();

    if (Projection == ::component::Camera::CameraProjection::PERSPECTIVE) {
      entity.AddComponent<::component::Camera>(fov, aspect, z_near, z_far);
    } else if (Projection == ::component::Camera::CameraProjection::ORTHOGRAPHIC) {
      entity.AddComponent<::component::Camera>(left, right, bottom, top, z_near, z_far);
    } else {
      CORE_ERROR("Unexpection CameraProjection...");
    }
  }

  auto camerafps_component = doc["CameraFPSComponent"];
  if (camerafps_component) {
    auto Projection = camerafps_component["Projection"].as<int>();
    auto fov = camerafps_component["Fov"].as<float>();
    auto aspect = camerafps_component["Aspect"].as<float>();
    auto left = camerafps_component["Left"].as<float>();
    auto right = camerafps_component["Right"].as<float>();
    auto bottom = camerafps_component["Bottom"].as<float>();
    auto top = camerafps_component["Top"].as<float>();
    auto z_near = camerafps_component["Znear"].as<float>();
    auto z_far = camerafps_component["Zfar"].as<float>();

    if (Projection == ::component::CameraFps::CameraProjection::PERSPECTIVE) {
      entity.AddComponent<::component::CameraFps>(fov, aspect, z_near, z_far);
    } else if (Projection == ::component::CameraFps::CameraProjection::ORTHOGRAPHIC) {
      entity.AddComponent<::component::CameraFps>(left, right, bottom, top, z_near, z_far);
    } else {
      CORE_ERROR("Unexpection CameraProjection...");
    }
  }

  auto direction_light_component = doc["DirectionLightComponent"];
  if (direction_light_component) {
    auto color = direction_light_component["Color"].as<glm::vec3>();
    auto intensity = direction_light_component["Intensity"].as<float>();
    entity.AddComponent<::component::DirectionLight>(color, intensity);
  }

  auto point_light_component = doc["PointLightComponent"];
  if (point_light_component) {
    auto color = point_light_component["Color"].as<glm::vec3>();
    auto intensity = point_light_component["Intensity"].as<float>();
    auto linear = point_light_component["Linear"].as<float>();
    auto quadratic = point_light_component["Quadratic"].as<float>();
    auto& point_light = entity.AddComponent<::component::PointLight>(color, intensity);
    point_light.set_attenuation(linear, quadratic);
  }

  auto spot_light_component = doc["SpotLightComponent"];
  if (spot_light_component) {
    auto color = spot_light_component["Color"].as<glm::vec3>();
    auto intensity = spot_light_component["Intensity"].as<float>();
    auto range = spot_light_component["Range"].as<float>();
    auto inner_cutoff = spot_light_component["Inner_cutoff"].as<float>();
    auto outer_cutoff = spot_light_component["Outer_cutoff"].as<float>();
    auto& spot_light = entity.AddComponent<::component::Spotlight>(color, intensity);
    spot_light.set_cutoff(range, inner_cutoff, outer_cutoff);
  }
  return entity;
}

}  // namespace scene
