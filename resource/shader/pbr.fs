#version 330 core

#define PI 3.14159265

#define INV_PI 0.31830988


layout (location = 0) out vec4 color;

in vec2 tex_coords;

in _vtx {
    in vec3 _position;
    in vec3 _normal;
    in vec2 _uv;
    in vec2 _uv2;
    in vec3 _tangent;
    in vec3 _binormal;
};
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
uniform float specular;

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
    float specular;
    float ao;
    vec3  diffuse_color;

    float alpha;

    float NoV;
    vec3 F0;
};

/*********************************** 光照 ***********************************/
layout(std140) uniform DL {
    vec4  color;
    vec4  direction;
    float intensity;
} dl;

/*********************************** camera ***********************************/
layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
    vec4 position;
    vec4 direction;
}camera;


/* 计算TBN矩阵
   T切线，B副切线，N法线
   参考 : http://www.thetenthplanet.de/archives/1180
*/

mat3 ComputeTBN(const vec3 position, const vec3 normal, const vec2 uv) {

    vec3 dp1 = dFdx(position); 
    vec3 dp2 = dFdy(position); 
    vec2 duv1 = dFdx(uv); 
    vec2 duv2 = dFdy(uv);   
    
    vec3 N = normalize(normal);
    vec3 dp2perp = cross(dp2, N); 
    vec3 dp1perp = cross(N, dp1); 
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x; 
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;   
    
    float invmax = inversesqrt(max(dot(T,T), dot(B,B))); 
    return mat3( T * invmax, B * invmax, N);

}


/* 材质的基色受材质自身的"金属度"影响. 电介质具有单色镜面反射, 
   但仍保留其基色作为漫反射颜色. 
   另一方面, 导体使用其基色作为镜面反射颜色, 但没有漫反射分量
   应用 : fliament 4.11.8.1 基色重映射 
*/
vec3 ComputeAlbedo(const vec3 albedo, float metalness) {
    return albedo * (1.0 - metalness);
}


/* 镜面反射对并且对电介质而言是单色的. 我们将电介质表面的重映射:
   f0 = 0.16 * reflectance * reflectance
   金属表面的镜面反射是多色的, 电介质而言是单色的
   所以我们在多色和单色间插值
   引用 : filament 4.11.8.2 反射率重映射
*/
vec3 ComputeF0(const vec3 albedo, float metalness, float specular) {
    vec3 dielectric_F0 = vec3(0.16 * specular * specular);
    return mix(dielectric_F0, albedo, metalness);
}


/* 简单的漫反射Lambertian BRDF, 它假定微面片半球具有均匀的漫反射
   即均分为 1 / PI
*/
float Fd_Lambert() {
    return INV_PI;
}

/* 迪斯尼漫反射BRDF 虑了粗糙度, 并在掠射角处添加了一些后向反射
   引用 : fliament 4.8 漫反射BRDF
*/
float Fd_Burley(float alpha, float NoV, float NoL, float HoL) {
    float F90 = 0.5 + 2.0 * HoL * HoL * alpha;
    float a = 1.0 + (F90 - 1.0) * pow(1.0 - NoL, 5);
    float b = 1.0 + (F90 - 1.0) * pow(1.0 - NoV, 5);
    return a * b * INV_PI;
}


/* 计算镜面反射BRDF
   参考: 4.9 标准模型的总结 
   https://jerkwin.github.io/filamentcn/Filament.md.html#%E6%9D%90%E8%B4%A8%E7%B3%BB%E7%BB%9F/%E5%8F%82%E6%95%B0%E5%8C%96
*/

/* 法向分布函数(镜面D) GGX分布是一种在高光中具有长尾衰减和短峰的分布
   公式 : fliament (4)
   参考 : fliament 4.7.1 法向分布函数(镜面D)
*/ 
float D_GGX(float NoH, float alpha) {
    float a = NoH * alpha;
    float k = alpha / (1.0 - NoH * NoH + a * a);
    return k * k * INV_PI;

        // float a2 = a * a;
        // float f = (NoH * a2 - NoH) * NoH + 1.0;
        // return a2 / (PI * f * f);
}


/* 几何阴影(镜面G)
   公式 ：fliament (15)
*/
float V_SmithGGXCorrelated(float NoV, float NoL, float a) {
    float a2 = a * a;
    float GGXL = NoV * sqrt((-NoL * a2 + NoL) * NoL + a2);
    float GGXV = NoL * sqrt((-NoV * a2 + NoV) * NoV + a2);
    return 0.5 / (GGXV + GGXL);
}

/* Fresnel(镜面F）
   这种效应模拟了这样一个事实, 观察者看到的由表面反射的光的多少取决于观察角度和材料的折射率
   沿法向入射时, 可以根据IOR计算出反射回来的光量f0
*/
vec3 F_Schlick(float VoH, vec3 f0) {
    return f0 + (vec3(1.0) - f0) * pow(1.0 - VoH, 5.0);
}

/* 镜面BRDF
   fr(v,l)=D(h,α)G(v,l,α)F(v,h,f0) / 4(n⋅v)(n⋅l)
*/
vec3 EvalSpecularLobe(const Pixel px, const vec3 L, const vec3 H, float NoV, float NoL, float NoH, float HoL) {
    float D = 0.0;
    float V = 0.0;
    vec3  F = vec3(0.0);

    D = D_GGX(NoH, px.alpha);
    V = V_SmithGGXCorrelated(NoV, NoL, px.alpha);
    F = F_Schlick(HoL, px.F0);

    return (D * V) * F;
}

// 漫反射 BRDF
vec3 EvalDiffuseLobe(const Pixel px, float NoV, float NoL, float HoL) {
    return px.diffuse_color * Fd_Burley(px.alpha, NoV, NoL, HoL);
}

vec3 EvalLobe(const Pixel px, vec3 L) {
    
    float NoL = dot(px.N, L);
    if (NoL <= 0.0) return vec3(0.0);

    vec3 H = normalize(px.V + L);
    vec3 Fr = vec3(0.0);
    vec3 Fd = vec3(0.0);
    vec3 Lo = vec3(0.0);

    float NoV = px.NoV;
    float NoH = max(dot(px.N, H), 0.0);
    float HoL = max(dot(H, L), 0.0);

    Fr = EvalSpecularLobe(px, L, H, NoV, NoL, NoH, HoL);  // compensate energy
    Fd = EvalDiffuseLobe(px, NoV, NoL, HoL);
    Lo = (Fd + Fr) * NoL;

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
    px.specular = clamp(specular, 0.35, 1.0);

    px.diffuse_color = ComputeAlbedo(px.albedo.rgb, px.metalness);
    px.F0 = ComputeF0(px.albedo.rgb, px.metalness, px.specular);
}

void main() {  

    Pixel px;
    px._position = _position;
    px._normal   = _normal;
    px._uv       = _uv;
    px._has_tbn  = true;
    
    InitPixel(px, camera.position.xyz);
    
    vec3 Lo = vec3(0.0);

    Lo += EvalLobe(px, dl.direction.xyz) * dl.color.rgb * dl.intensity;
    
    color = vec4(Lo, px.albedo.a);
}
