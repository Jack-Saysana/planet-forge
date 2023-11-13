const char *vertex_shader =
"#version 430 core\n"
"layout (location = 0) in vec3 in_pos;\n"
"layout (location = 0) in vec3 in_norm;\n"
"layout (location = 0) in vec2 in_tex_coord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 proj;\n"
"out vec3 frag_norm;\n"
"void main() {\n"
"  gl_Position = proj * view * model * vec4(in_pos, 1.0);\n"
"  gl_PointSize = 1.0;\n"
"  frag_norm = in_norm;\n"
"}\n";

const char *fragment_shader =
"#version 430 core\n"
"out vec4 FragColor;\n"
"in vec3 frag_norm;\n"
"void main() {\n"
"  FragColor = vec4(frag_norm, 1.0);\n"
"}\n";
