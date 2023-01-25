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

vec3 PrefilterEnvironmentMap(vec3 R, uint n_samples) {
     vec3 N = R;
     vec3 V = R;

     vec2 env_size = vec2(textureSize(environment_map, 0));
     float w = 4.0 * PI / (6 * env_size.x * env_size.y);

     // roughness is guaranteed to be > 0 as the base level is copied directly
     float alpha = roughness * roughness;

     float weight = 0.0;
     vec3 color = vec3(0.0);

     for (uint i = 0U; i < n_samples; i++) {
         vec2 u = Hammersley2D(i, n_samples);
         vec3 H = Tangent2World(N, ImportanceSampleGGX(u.x, u.y, alpha));
         vec3 L = 2 * dot(H, V) * H - V;

         float NoH = max(dot(N, H), 0.0);
         float NoL = max(dot(N, L), 0.0);
         float HoV = max(dot(H, V), 0.0);

         if (NoL > 0.0) {
             float pdf = D_TRGGX(NoH, alpha) * NoH / (4.0 * HoV);
             float ws = 1.0 / (n_samples * pdf + 0.0001);  // solid angle associated with this sample (Equation 11)
             float mip_level = max(0.5 * log2(ws / w) + 1.0, 0.0);  // mipmap level (Equation 13 biased by +1)
             color += textureLod(environment_map, L, mip_level).rgb * NoL;
             weight += NoL;
         }
     }

     return color / weight;
}

void main() {
  vec3 R = tex_coords;
  color = vec4(PrefilterEnvironmentMap(R, 1024U), 1.0);
}

