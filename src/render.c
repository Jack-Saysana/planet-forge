#include <render.h>

void init_scene() {
  test_shader = init_shader(vertex_shader, NULL, fragment_shader);

  VERT test_verts[] = {
    {{ 1.0,  1.0, -10.0}, {0.0, 0.0, 1.0}, { 1.0,  1.0}},
    {{ 1.0, -1.0, -10.0}, {0.0, 0.0, 1.0}, { 1.0, -1.0}},
    {{-1.0, -1.0, -10.0}, {0.0, 0.0, 1.0}, {-1.0, -1.0}},
    {{-1.0,  1.0, -10.0}, {0.0, 0.0, 1.0}, {-1.0,  1.0}}
  };

  ivec3 test_inds[] = {
    {0, 1, 2},
    {2, 3, 0}
  };

  MESH_DATA md = {
    test_verts,
    test_inds,
    4,
    2
  };

  init_model(&test_model, &md);
  sphere = gen_sphere();

  glm_ortho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0, ortho_proj);
  glm_perspective(glm_rad(45.0f), ((float) RES_X) / ((float) RES_Y), 0.1,
                  100.0, persp_proj);

  glEnable(GL_PROGRAM_POINT_SIZE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);
}

void render_scene(GLFWwindow *window) {
  float current_frame = glfwGetTime();
  delta_time = current_frame - last_frame;
  last_frame = current_frame;

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //test_render();
  render_sphere();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void test_render() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 view = GLM_MAT4_IDENTITY_INIT;

  glUseProgram(test_shader);
  set_mat4("model", model, test_shader);
  set_mat4("view", view, test_shader);
  set_mat4("proj", persp_proj, test_shader);
  draw_model(&test_model, test_shader);
}

void render_sphere() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model, (vec3) {0.0, 0.0, -5.0});
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  calc_cam_space(view);

  glUseProgram(test_shader);
  set_mat4("model", model, test_shader);
  set_mat4("view", view, test_shader);
  set_mat4("proj", persp_proj, test_shader);
  //draw_points(sphere);
  draw_model(sphere, test_shader);
}

unsigned int init_shader(const char *vs, const char *gs, const char *fs) {
  int vs_len = strlen(vs);
  unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vs, &vs_len);
  glCompileShader(vert_shader);
  int status = 0;
  char info_log[1024];
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int info_log_len = 0;
    glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &info_log_len);
    glGetShaderInfoLog(vert_shader, 1024, &info_log_len, info_log);
    fprintf(stderr, "Vertex shader error:\n%s\n", info_log);
     return INVALID_SHADER;
  }

  unsigned int geo_shader = INVALID_SHADER;
  if (gs) {
    int gs_len = strlen(fs);
    geo_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geo_shader, 1, &gs, &gs_len);
    glCompileShader(geo_shader);
    glGetShaderiv(geo_shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      int info_log_len = 0;
      glGetShaderiv(geo_shader, GL_INFO_LOG_LENGTH, &info_log_len);
      glGetShaderInfoLog(geo_shader, 1024, &info_log_len, info_log);
      fprintf(stderr, "Geometry shader error:\n%s\n", info_log);
      return INVALID_SHADER;
    }
  }

  int fs_len = strlen(fs);
  unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &fs, &fs_len);
  glCompileShader(frag_shader);
  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    int info_log_len = 0;
    glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &info_log_len);
    glGetShaderInfoLog(frag_shader, 1024, &info_log_len, info_log);
    fprintf(stderr, "Fragment shader error:\n%s\n", info_log);
    return INVALID_SHADER;
  }

  unsigned int shader_prog = glCreateProgram();
  glAttachShader(shader_prog, vert_shader);
  glAttachShader(shader_prog, frag_shader);
  if (gs) {
    glAttachShader(shader_prog, geo_shader);
  }
  glLinkProgram(shader_prog);
  glDeleteShader(vert_shader);
  glDeleteShader(frag_shader);
  if (gs) {
    glDeleteShader(geo_shader);
  }
  return shader_prog;
}

void set_mat4(char *name, mat4 matrix, unsigned int shader) {
  glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE,
                     (float *) matrix);
}
