#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D texture_0;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;

void main()
{    
    color = vec4(texture(texture_0, tex_coords).rgb, 1.0);
}
