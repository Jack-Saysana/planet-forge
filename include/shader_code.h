const char *vertex_shader =
"#version 430 core\n"
"layout (location = 0) in vec3 in_pos;\n"
"layout (location = 0) in vec3 in_norm;\n"
"layout (location = 0) in vec2 in_tex_coord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 proj;\n"
"out vec3 frag_norm;\n"
"out vec3 frag_pos;\n"
"void main() {\n"
"  gl_Position = proj * view * model * vec4(in_pos, 1.0);\n"
"  gl_PointSize = 1.0;\n"
"  frag_norm = in_norm;\n"
"  frag_pos = vec3(model * vec4(in_pos, 1.0));\n"
"}\n";

const char *fragment_shader =
"#version 430 core\n"
"out vec4 FragColor;\n"
"in vec3 frag_norm;\n"
"in vec3 frag_pos;\n"
"uniform vec3 light_pos;\n"
"uniform vec3 sphere_center;\n"
"uniform float radius;\n"
"float shadow() {\n"
"  vec3 normal_to_light = normalize(light_pos - frag_pos);\n"
"  return max(dot(normal_to_light, frag_norm), 0.0);\n"
"}\n"
"float dist_from_center() {\n"
"  return distance(frag_pos, sphere_center);\n"
"}\n"
"void main() {\n"
"  vec3 r = vec3(1.0, 0.0, 0.0);\n"
"  vec3 g = vec3(0.0, 1.0, 0.0);\n"
"  vec3 b = vec3(0.0, 0.0, 1.0);\n"
"  float dist = dist_from_center();\n"
"  if (dist > 0.0 && dist < 1.15 * radius) {\n"
"    FragColor = vec4(mix(r, g, (dist - radius) / (0.15 * radius)), 1.0) * shadow();\n"
"  } else if (dist >= 1.15 * radius && dist < radius * 1.75) {\n"
"    FragColor = vec4(mix(g, b, (dist - (radius * 1.15)) / (radius * 0.5)), 1.0) * shadow();\n"
"  } else {\n"
"    FragColor = vec4(b, 1.0) * shadow();\n"
"  }\n"
"}\n";
