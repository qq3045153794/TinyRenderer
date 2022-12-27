#version 330 core
in vec3 tex_coords;
out vec4 color;

uniform samplerCube texture_0;
void main()
{    
    color = vec4(texture(texture_0, tex_coords).rgb, 1.0);
}