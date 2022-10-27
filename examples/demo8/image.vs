#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_nor;
layout (location = 2) in vec2 a_tex;

out vec2 tex_coords;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position =  model * vec4(a_pos, 1.0);
    tex_coords = a_tex;
}