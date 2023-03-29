#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec2 uv2;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 binormal;


out _vtx {
    out vec3 _position;
    out vec3 _normal;
    out vec2 _uv;
    out vec2 _uv2;
    out vec3 _tangent;
    out vec3 _binormal;
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
    _tangent  = normalize(vec3(model * vec4(tangent, 0.0)));
    _binormal = normalize(vec3(model * vec4(binormal, 0.0)));
    _uv = uv;
}
