#version 330 core
out vec4 color;

uniform sampler2D texture_0;
in vec2 uv;

void main () {
    vec3 RGB =  texture(texture_0, uv).rgb;
    float R = RGB.r;
    float G = RGB.g;
    float B = RGB.b;
    // Y 范围[0 1] 灰阶值
    float Y = 0.2627 * R + 0.6780 * G + 0.0593 * B;
    color = vec4(Y, Y, Y, 1.0);
}