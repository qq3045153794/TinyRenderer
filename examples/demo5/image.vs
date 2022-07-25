#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_tex;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec2 tex_coords;

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0);
    tex_coords = a_tex;
}