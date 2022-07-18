#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 pos;
in vec3 N;

uniform sampler2D texture_diffuse1;

uniform samplerCube tex_cube;
uniform vec3 V;

void main()
{    
    float ratio = 1.00 / 1.33;
    vec3 I = normalize(pos - V);
    vec3 R = refract(I, normalize(N), ratio);
    
    FragColor = vec4(texture(tex_cube, R).rgb, 1.0);
}
