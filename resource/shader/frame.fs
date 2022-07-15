#version 330 core
in vec2 tex_coords;
out vec4 color;

uniform sampler2D texture_0;
uniform int process;
const float offset = 1.0 / 600.0; 

vec4 YUV() {
    vec3 RGB =  texture(texture_0, tex_coords).rgb;
    float R = RGB.r;
    float G = RGB.g;
    float B = RGB.b;
    // Y 范围[0.0 1]
    float Y = 0.2627 * R + 0.6780 * G + 0.0593 * B;
    // U V 范围[-0.5 0.5]
    float U = -0.1396 * R + 0.3604 * G + 0.5 * B + 0.5;
    float V = 0.5 * R - 0.4598 * G - 0.0402 * B + 0.5;
    return vec4(Y, Y, Y, 1.0);
}

vec4 gaussian_blur() {
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        0.0453, 0.0566, 0.0453,
        0.0566, 0.0707, 0.0566,
        0.0453, 0.0566, 0.0453 
    );

    vec3 sample_tex[9];
    for(int i = 0; i < 9; i++) {
        sample_tex[i] = vec3(texture(texture_0, tex_coords + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++) {
        col += sample_tex[i] * kernel[i];
    }
    return vec4(col, 1.0);
}

void main() { 
    
    if(process == 1) {
        color = YUV();
    } else if(process ==2 ){
        color = gaussian_blur();        
    } else {
        color = vec4(texture(texture_0, tex_coords).rgb,1.0);
    }

}
