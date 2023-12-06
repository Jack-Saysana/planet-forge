#version 430 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_bitangent;
layout (location = 4) in vec2 in_tex_coord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 frag_norm;
out vec3 frag_pos;
out mat4 view_mat;
out vec2 tex_coords;
out vec3 tangent;
out vec3 bitangent;
void main() {
  gl_Position = proj * view * model * vec4(in_pos, 1.0);
  gl_PointSize = 1.0;
  frag_norm = mat3(transpose(inverse(model))) * in_norm;
  frag_pos = vec3(model * vec4(in_pos, 1.0));
  view_mat = view;
  tex_coords = in_tex_coord;
  tangent = mat3(transpose(inverse(model))) * in_tangent;
  bitangent = mat3(transpose(inverse(model))) * in_bitangent;
}