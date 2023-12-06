#version 430 core
out vec4 FragColor;
in vec3 frag_norm;
in vec3 frag_pos;

uniform vec3 light_pos;
uniform vec3 planet_pos;
uniform vec3 view_pos;

float calc_lighting();
float calc_opacity();
vec3 calc_color();

void main() {
  float lighting = calc_lighting();
  //lighting = 1.0;\n"
  vec3 color = calc_color();
  vec3 res = lighting * color;
  float opacity = calc_opacity();
  FragColor = vec4(res, opacity);
}

float calc_lighting() {
  float ambient = 0.1;

  vec3 norm = normalize(frag_norm);
  vec3 light_dir = normalize(light_pos - frag_pos);
  float diff = max(dot(light_dir, norm), 0.0);

  return diff + ambient;
}

vec3 calc_color() {
  float atmo_dist = distance(frag_pos, planet_pos);
  float view_dist = distance(view_pos, planet_pos);
  if (atmo_dist - view_dist > 0.0) {
    vec3 norm = normalize(view_pos - planet_pos);
    vec3 light_dir = normalize(light_pos - view_pos);
    float diff = 0.25 + max(dot(light_dir, norm), 0.0);

    vec3 cam_dir = normalize(view_pos - planet_pos);
    vec3 to_atmos = normalize(frag_pos - view_pos);
    float val = max(dot(cam_dir, to_atmos), -diff);
    val = min(val + diff, 1.0);
    vec3 col = (val * vec3(0.0117, 0.8745, 0.9882)) +
               ((1.0 - val) * vec3(1.0, 0.6392, 0.2313));
    return col;
  } else {
    return vec3(0.0117, 0.8745, 0.9882);
    //return vec3(1.0, 0.0, 0.0);
  }
}

float calc_opacity() {
  float atmo_dist = distance(frag_pos, planet_pos);
  float view_dist = distance(view_pos, planet_pos);
  float diff = min(max(atmo_dist - view_dist, 0.1), 0.75);
  return diff;
}
