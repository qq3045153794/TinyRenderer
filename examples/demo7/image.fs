#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D texture_0;

void main()
{    
    color = vec4(texture(texture_0, tex_coords).rgb, 1.0);
}
