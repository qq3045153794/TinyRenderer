#version 330 core
in vec3 tex_coords;
out vec4 color;

uniform samplerCube texture_0;

#define PI 3.1415926

vec3 Tangent2World(vec3 N, vec3 V) {
  N = normalize(N);

  vec3 U = normalize(mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), step(abs(N.y), 0.999)));
  vec3 T = normalize(cross(U, N));
  vec3 B = normalize(cross(N, T));

  return T * V.x + B * V.y + N * V.z;
}

void main() {

  vec3 irradiance = vec3(0.0);
  vec3 N = normalize(tex_coords);
  float sampleDelta = 0.025;
  float nrSamples = 0.0;

  for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)  {
      for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
      {
          // spherical to cartesian (in tangent space)
          vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
          // tangent space to world
          vec3 sampleVec = Tangent2World(N, tangentSample);

          irradiance += texture(texture_0, sampleVec).rgb * cos(theta) * sin(theta);
          nrSamples++;
      }
  }
  irradiance = PI * irradiance * (1.0 / float(nrSamples));
  // vec3 envColor = texture(texture_0, uv).rgb;
  color = vec4(irradiance, 1.0);
}
