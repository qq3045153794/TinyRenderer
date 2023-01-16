#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_nor;
layout (location = 2) in vec2 a_tex;

out vec3 tex_coords;
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main()
{
    mat4 rectified_view = mat4(mat3(view));

    vec4 pos = projection * rectified_view * model * vec4(a_pos, 1.0);

    gl_Position = pos.xyww;
    // gl_Position = pos.xyzw;
    tex_coords = a_pos;
}
