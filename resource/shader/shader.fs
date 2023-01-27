#version 330 core
out vec4 color;

in _vtx {
    in vec3 _position;
    in vec3 _normal;
    in vec2 _uv;
};

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
    vec4 position;
    vec4 direction;
}camera;

uniform sampler2D texture_0;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;

void main()
{
    color = vec4(texture(texture_0, _uv).rgb,  1.0);
}
