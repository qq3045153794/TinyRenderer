#version 330 core
in vec3 tex_coords;
out vec4 color;

uniform samplerCube texture_0;
void main()
{
    vec3 envColor = texture(texture_0, tex_coords).rgb;
    // vec3 envColor = textureLod(texture_0, tex_coords, 1.2).rgb;
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));
    color = vec4(envColor, 1.0);
}
