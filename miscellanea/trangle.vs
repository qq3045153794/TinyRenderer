#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_coord;
out vec2 tex_coord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(a_Pos, 1.0);
  tex_coord = a_coord;
}
