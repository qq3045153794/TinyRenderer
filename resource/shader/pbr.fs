#version 330 core

#define PI 3.1415926


in vec2 tex_coords;
layout (location = 0) out vec4 color;


/*********************************** 材质属性 ***********************************/
uniform bool sample_albedo;
uniform bool sample_metalness;
uniform bool sample_roughness;
uniform bool sample_ao;
uniform bool sample_normal;

uniform vec4 albedo;
uniform float metalness;
uniform float roughness;
uniform float ao;

uniform sampler2D albedo_texture;
uniform sampler2D metalness_texture;
uniform sampler2D roughness_texture;
uniform sampler2D ao_texture;
uniform sampler2D normal_texture;

uniform sampler2D texture_0;
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;

struct Pixel {
    vec3 _position;
    vec3 _normal;
    vec2 _uv;
    vec2 _uv2;
    vec3 _tangent;
    vec3 _binormal;
    
    bool _has_tbn;
    bool _has_uv2;
    
    vec3 position;
    vec3 normal;
    vec2 uv;
    mat3 TBN;

    vec3 V;
    vec3 N;
    vec3 R;
    vec3 Nov;

    vec4  albedo;
    float metalness;
    float roughness;
    float ao;
    vec4  diffuse_color

    float alpha;

    float NoV;
    vec3 F0;
};


/* 计算镜面反射BRDF
   参考: 4.9 标准模型的总结 
   https://jerkwin.github.io/filamentcn/Filament.md.html#%E6%9D%90%E8%B4%A8%E7%B3%BB%E7%BB%9F/%E5%8F%82%E6%95%B0%E5%8C%96
*/
float D_GGX(float NoH, float a) {
    float a2 = a * a;
    float f = (NoH * a2 - NoH) * NoH + 1.0;
    return a2 / (PI * f * f);
}

vec3 F_Schlick(float VoH, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * pow(1.0 - VoH, 5.0);
}

float V_SmithGGXCorrelated(float NoV, float NoL, float a) {
    float a2 = a * a;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

float Fd_Lambert() {
    return 1.0 / PI;
}


mat3 ComputeTBN(const vec3 position, const vec3 normal, const vec2 uv) {
    return mat3(1.0);
}

vec3 ComputeAlbedo(const vec3 albedo, float metalness) {
    return albedo * (1.0 - metalness);
}

vec3 ComputeF0(const vec3 albedo, float metalness, float specular) {
    vec3 dielectric_F0 = vec3(0.16 * specular * specular);
    return mix(dielectric_F0, albedo, metalness);
}

void InitPixel(inout Pixel px, const vec3 camera_pos) {
    px.position = px._position;
    px.uv = px._uv;
    px.TBN = px._has_tbn? mat3(px._tangent, px._binormal, px._normal) : ComputeTBN(px._position, px._normal, px.uv);


    px.V = normalize(camera_pos - px.position);
    px.N = sample_normal ? normalize(px.TBN * (texture(normal_texture, px.uv).rgb * 2.0 - 1.0)) : px._normal;
    px.R = reflect(-px.V, px.N);
    px.NoV = max(dot(px.N, px.V), 1e-4);

    px.albedo = sample_albedo ? vec4(texture(albedo_texture, px.uv).rgb, 1.0) : albedo;


    // 将粗糙度其重新映射到感知线性范围
    // 引用: Filament 文档公式(37)
    px.roughness = sample_roughness ? texture(roughness_texture, px.uv).r : roughness;
    px.roughness = clamp(px.roughness, 0.045, 1.0);
    px.alpha = px.roughness * px.roughness;

    px.ao = sample_ao ? texture(ao_texture, px.uv).r : 1.0;


    px.metalness = sample_metalness ? texture(metalness_texture, px.uv).r : metalness;

    px.diffuse_color = ComputeAlbedo(px.albedo.rgb, px.metalness);
    px.F0 = ComputeF0(px.albedo.rgb, px.metalness, px.specular);
}

void main() {    
    color = vec4(texture(texture_0, tex_coords).rgb, 1.0);
}
