#version 430 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;
in VS_OUT {
  vec3 tangent;
  vec3 bitangent;
  vec3 normal;
} gs_in[];

uniform mat4 proj;

void main() {
  vec3 intermediate = vec3(gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position);
  intermediate *= 0.3333;
  gl_Position = proj * vec4(intermediate, 1.0);
  EmitVertex();
  gl_Position = proj * (vec4(intermediate, 1.0) + (0.05 * vec4(gs_in[0].normal, 1.0)));
  EmitVertex();

  gl_Position = proj * vec4(intermediate, 1.0);
  EmitVertex();
  gl_Position = proj * (vec4(intermediate, 1.0) + (0.01 * vec4(gs_in[0].bitangent, 1.0)));
  EmitVertex();

  gl_Position = proj * vec4(intermediate, 1.0);
  EmitVertex();
  gl_Position = proj * (vec4(intermediate, 1.0) + (0.005 * vec4(gs_in[0].tangent, 1.0)));
  EmitVertex();
  EndPrimitive();
}