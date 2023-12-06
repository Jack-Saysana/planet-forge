#version 430 core
#define PI 3.14159
out vec4 FragColor;
in vec3 frag_norm;
in vec3 frag_pos;
in vec2 tex_coords; 
in vec3 tangent;
in vec3 bitangent;
in mat3 model_mat;
uniform vec3 light_pos;
uniform float time;
uniform vec3 camera_position;
uniform sampler2D norm_tex;
uniform int texture_enabled;
vec4 shadow(vec3 norm, vec4 frag_col) {
  float ambient_str = 0.05;
  vec3 ambient = ambient_str * vec3(frag_col);
  float spec_strength = 0.9;
  vec3 light_dir = normalize(light_pos - frag_pos);
  vec3 view_dir = normalize(camera_position - frag_pos);
  vec3 diffuse = vec3(frag_col) * max(dot(light_dir, norm), 0.0);
  vec3 reflection_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflection_dir), 0.0), 32) *
               min(3.0 * max(dot(view_dir, norm), 0.0), 1.0) * spec_strength;
  vec3 specular = spec * vec3(1.0);
  return vec4(ambient + diffuse + specular, 1.0);
}

float clamp_fl(float i) {
  return 0.5 * (i + 1.0);
}

void main() {
  mat3 TBN = mat3(tangent, bitangent, frag_norm);
  float xper = 0.0;
  float yper = 0.0;
  float a = 0.15;
  float b = 0.05;
  vec3 new_normal = vec3(0.0);
  if (texture_enabled == 0) {
    float n = (sin((tex_coords.y / b) + time) + sin((tex_coords.x / b) + time))/2.0;
    xper = 
         a * (((sin(((PI * tex_coords.x) / b) + time) + n) * 
         (sin(sin(time + ((tex_coords.x * tex_coords.y) / b))))));
    yper = 0.0;
    xper = clamp(xper, yper, 1.0);
    yper = xper;
    vec3 t = normalize(vec3(xper, yper, 1.0));
    new_normal = normalize(TBN * t);
  } else {
    vec2 offset = vec2(time * 0.005);
    vec3 sam = vec3(texture(norm_tex, tex_coords + offset));
    new_normal = normalize(TBN * sam);
  }
  FragColor = vec4(0.0, 0.0, 1.0, 1.0);
  FragColor = vec4(vec3(shadow(normalize(new_normal), FragColor)), 0.75);
}