#version 430 core
out vec4 FragColor;
in vec3 frag_norm;
in vec3 frag_pos;
uniform vec3 light_pos;
uniform vec3 sphere_center;
uniform vec3 camera_position;
uniform float radius;

vec4 shadow(vec3 norm, vec4 frag_col) {
  float ambient_str = 0.05;
  vec3 ambient = ambient_str * vec3(frag_col);
  float spec_strength = 0.5;
  vec3 light_dir = normalize(light_pos - frag_pos);
  vec3 view_dir = normalize(camera_position - frag_pos);
  vec3 diffuse = vec3(frag_col) * max(dot(light_dir, norm), 0.0);
  vec3 reflection_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflection_dir), 0.0), 1) *
               min(3.0 * max(dot(view_dir, norm), 0.0), 1.0) * spec_strength;
  vec3 specular = spec * vec3(frag_col);
  return vec4(ambient + diffuse + specular, 1.0);
}

float dist_from_center() {
  return distance(frag_pos, sphere_center);
}

void main() {
  vec3 br = vec3(0.392, 0.349, 0.325);
  vec3 dark_green = vec3(0.0, 10.0 / 255.0, 0.0);
  vec3 g = vec3(0.0, 1.0, 0.0);
  vec3 b = vec3(0.0, 0.0, 1.0);
  vec3 w = vec3(1.0, 1.0, 1.0);
  float dist = dist_from_center();
  if (dist > 0.0 && dist < 1.025 * radius) {
    FragColor = vec4(mix(dark_green, g, (dist - radius) / (0.025 * radius)), 1.0);
  } else if (dist >= 1.025 * radius && dist < radius * 1.10) {
    FragColor = vec4(mix(g, br, (dist - (radius * 1.025)) / (radius * 0.075)), 1.0);
  } else if (dist >= 1.10 * radius && dist < 1.25 * radius) {
    FragColor = vec4(mix(br, w, (dist - (radius * 1.1)) / (radius * 0.15)), 1.0);
  } else {
    FragColor = vec4(w, 1.0);
  }
  FragColor = shadow(normalize(frag_norm), FragColor);
}
