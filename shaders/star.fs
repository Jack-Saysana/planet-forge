#version 430 core
out vec4 FragColor;
in vec3 frag_pos;
in vec2 tex_coords; 
uniform float time;
float rand(vec2 uv) {
  const float a = 12.9898;
  const float b = 78.233;
  const float c = 43758.5453;
  float dt = dot(uv, vec2(a, b));
  float sn = mod(dt, 3.15159);
  return fract(sin(sn) * c);
}
float draw_stars() {
  float res = 2048.0;
  vec2 pixel_coords = floor(tex_coords * res) / res;
  float t = sin(time * 2.0 * rand(-pixel_coords)) * 0.5 + 0.5;
  return smoothstep(0.975, 1.0, rand(pixel_coords)) * t;
}
void main() {
  float amount = draw_stars();
  FragColor = vec4(vec3(amount), 0.5);
}