#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D texture_1;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

void main()
{    
    color = vec4(texture(texture_1, tex_coords).rgb, 1.0);
}
