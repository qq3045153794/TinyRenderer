
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec2 uv2;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 binormal;
layout(location = 6) in ivec4 bone_id;
layout(location = 7) in vec4 bone_wt;

uniform bool is_animation;
uniform mat4 bone_transform[600];

uniform mat4 model;
uniform mat4 light_space_matrix;

mat4 CalcBoneTransform() {
  mat4 T = mat4(0.0);
  for(uint i = 0U; i < 4U; i++) {
    if(bone_id[i] >= 0) {
      T += (bone_transform[bone_id[i]] * bone_wt[i]);
    }
  }
  return T;
}

void main()
{
    mat4 BT = is_animation? CalcBoneTransform() : mat4(1.0);
    gl_Position = light_space_matrix * model * BT * vec4(position, 1.0);
}
