#version 330 core
in vec3 tex_coords;
out vec4 color;

uniform float roughness;
uniform samplerCube environment_map;

#define PI 3.1415926
#define PI2 6.2831852
#define INV_PI 0.31830989

vec3 Tangent2World(vec3 N, vec3 V) {
  N = normalize(N);

  vec3 U = normalize(mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), step(abs(N.y), 0.999)));
  vec3 T = normalize(cross(U, N));
  vec3 B = normalize(cross(N, T));

  return T * V.x + B * V.y + N * V.z;
}


// Van Der Corput 序列 我们将用它来获得 Hammersley 序列
float RadicalInverse_VdC(uint bits) {
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// 蒙特卡洛积分 设总样本数为N 样本索引为 i
vec2 Hammersley2D(uint i, uint N) {
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(float u, float v, float alpha) {
    float a2 = alpha * alpha;
    float phi = u * PI2;
    float cos_theta = sqrt((1.0 - v) / (1.0 + (a2 - 1.0) * v));  // bias toward cosine and TRGGX NDF
    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    return vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
}

float D_TRGGX(float alpha, float NoH) {
    float a = NoH * alpha;
    float k = alpha / (1.0 - NoH * NoH + a * a);
    return k * k * INV_PI;
}

// Smith's height-correlated visibility function (V = G / normalization term)
// Heitz 2014, "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"
float V_SmithGGX(float alpha, float NoV, float NoL) {
    float a2 = alpha * alpha;
    float GGXV = NoL * sqrt(NoV * NoV * (1.0 - a2) + a2);
    float GGXL = NoV * sqrt(NoL * NoL * (1.0 - a2) + a2);
    return 0.5 / (GGXV + GGXL);
}

vec2 IntegrateBRDF(float NoV, float roughness, uint n_samples) {
    float alpha = roughness * roughness;
    float inv_ns = 1.0 / float(n_samples);

    NoV = max(NoV, 1e-4);  // reduce artifact on the border
    vec3 N = vec3(0.0, 0.0, 1.0);
    vec3 V = vec3(sqrt(1.0 - NoV * NoV), 0.0, NoV);  // vec3(sin, 0, cos)

    float scale = 0.0;
    float bias = 0.0;

    for (uint i = 0; i < n_samples; i++) {
       vec2 u = Hammersley2D(i, n_samples);
       vec3 H = ImportanceSampleGGX(u.x, u.y, alpha);  // keep in tangent space
       precise vec3 L = 2 * dot(V, H) * H - V;  // need the precise qualifier

       // implicitly assume that all vectors lie in the X-Z plane
       float NoL = max(L.z, 0.0);
       float NoH = max(H.z, 0.0);
       float HoV = max(dot(H, V), 0.0);

       if (NoL > 0.0) {
           float V = V_SmithGGX(alpha, NoV, NoL) * NoL * HoV / max(NoH, 1e-5);
           float Fc = pow5(1.0 - HoV);  // Fresnel F has been factorized out of the integral

          // scale += V * (1.0 - Fc);  // this only considers single bounce
          // bias += V * Fc;           // this only considers single bounce

          scale += V * Fc;  // take account of multi-scattering energy compensation
          bias += V;        // take account of multi-scattering energy compensation
       }
    }
    scale *= (4.0 * inv_ns);
    bias  *= (4.0 * inv_ns);

    return vec2(scale, bias);
}

void main () {
  float cosine    = tex_coords.x;
  float roughness = tex_coords.y;

  color = IntegrateBRDF(cosine, roughness, 1024u);
}


