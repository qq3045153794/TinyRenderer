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
    vec3 I = normalize(pos - V);
    vec3 R = reflect(I, normalize(N));
    
    FragColor = vec4(texture(tex_cube, R).rgb, 1.0);
}
