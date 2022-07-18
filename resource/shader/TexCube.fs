#version 330 core
out vec4 color;
in vec3 tex_coord;
uniform samplerCube texture1;
void main() {
  color = texture(texture1, tex_coord);
}