#version 430 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec3 in_norm;
layout (location = 2) in vec3 in_tangent;
layout (location = 3) in vec3 in_bitangent;
layout (location = 4) in vec2 in_tex_coord;
uniform mat4 model;
uniform mat4 view;
out VS_OUT {
  vec3 tangent;
  vec3 bitangent;
  vec3 normal;
} vs_out;
void main() {
  gl_Position = view * model * vec4(in_pos, 1.0);
  mat3 n_mat = mat3(transpose(inverse(view * model)));
  vs_out.normal = n_mat * in_norm;
  vs_out.tangent = n_mat * in_tangent;
  vs_out.bitangent = n_mat * in_bitangent;
}