#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D texture_0;

void main() { 
    vec3 RGB = texture(texture_0, tex_coords).rgb;
    float R = RGB.r;
    float G = RGB.g;
    float B = RGB.b;
    // Y 范围[0.0 1]
    float Y = 0.2627 * R + 0.6780 * G + 0.0593 * B;
    // U V 范围[-0.5 0.5]
    float U = -0.1396 * R + 0.3604 * G + 0.5 * B + 0.5;
    float V = 0.5 * R - 0.4598 * G - 0.0402 * B + 0.5;
    color = vec4(R, G, B, 1.0);

}
