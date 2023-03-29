#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;


out _vtx {
    out vec3 _position;
    out vec3 _normal;
    out vec2 _uv;
};


layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
    vec4 position;
    vec4 direction;
}camera;

uniform mat4 model;

void main()
{
    gl_Position = camera.projection * camera.view * model * vec4(position, 1.0);
    _position = vec3(model * vec4(position, 1.0));
    _normal   = normalize(vec3(model * vec4(normal, 0.0)));
    _uv = uv;
}
