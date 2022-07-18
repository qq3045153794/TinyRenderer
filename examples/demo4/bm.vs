#version 330 core

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec2 TexCoords;
out vec3 pos;
out vec3 N;

void main() {

    mat4 viewModel = view * model;
    pos = vec3(viewModel * vec4(a_pos, 1.0));
    gl_Position =  projection * viewModel * vec4(a_pos, 1.0);
	TexCoords = tex;
    N = mat3(transpose(inverse(model))) * norm;
}
