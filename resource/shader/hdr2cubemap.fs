#version 330 core
in vec3 tex_coords;
out vec4 color;

uniform sampler2D texture_0;

// const vec2 invAtan = vec2(0.1591, 0.3183);

#define PI 3.1415926
#define PI2 6.2831852

vec2 SampleSphericalMap(vec3 v)
{
    // vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    // uv *= invAtan;
    // uv += 0.5;
    // return uv;
    float phi = atan(v.z, v.x);
    float theta = acos(v.y);
    return vec2(phi / PI2 + 0.5, theta / PI);
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(tex_coords));
    vec3 envColor = texture(texture_0, uv).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2));

    color = vec4(envColor, 1.0);
}
