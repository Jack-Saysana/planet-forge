#include <render.h>

void init_scene() {
  test_shader = init_shader(vertex_shader, NULL, fragment_shader);
  ocean_shader = init_shader(vertex_shader, NULL, frag_ocean_shader);
  sun_shader = init_shader(vertex_shader, NULL, frag_sun_shader);
  normal_shader = init_shader(vertex_shader_norm, geom_shader, frag_shader_norm);

  sphere_mesh = gen_sphere();
  sphere = init_model(sphere_mesh);
  normal_tex = gen_texture("./maps/ocean_normal_map.jpg");
  ocean = init_model(sphere_mesh);
  sun = init_model(sphere_mesh);
  cube = gen_cube();
  refresh_sphere();

  glm_ortho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0, ortho_proj);
  glm_perspective(glm_rad(45.0f), ((float) RES_X) / ((float) RES_Y), 0.1,
                  100.0, persp_proj);

  glEnable(GL_PROGRAM_POINT_SIZE);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void render_scene(GLFWwindow *window) {
  float current_frame = glfwGetTime();
  delta_time = current_frame - last_frame;
  last_frame = current_frame;

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_sun();
  render_sphere();
  //render_cube();
  render_ocean();
  //render_normals();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void render_sun() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 view = GLM_MAT4_IDENTITY_INIT;

  light_pos[0] = (RADIUS + 3.0) * (cos(day_cycle * glfwGetTime())) + sphere_center[0];
  light_pos[1] = sphere_center[1];
  light_pos[2] = (RADIUS + 3.0) * (sin(day_cycle * glfwGetTime())) + sphere_center[2];

  glm_translate(model, light_pos);
  glm_scale(model, (vec3) {0.1, 0.1, 0.1});
  calc_cam_space(view);

  glUseProgram(sun_shader);

  set_mat4("model", model, sun_shader);
  set_mat4("view", view, sun_shader);
  set_mat4("proj", persp_proj, sun_shader);
  draw_model(sun, sun_shader);
}

void render_cube() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 view = GLM_MAT4_IDENTITY_INIT;

  vec3 cube_pos = { 0.0, 0.0, -5.0 };
  glm_translate(model, cube_pos);
  calc_cam_space(view);

  glUseProgram(sun_shader);

  set_mat4("model", model, sun_shader);
  set_mat4("view", view, sun_shader);
  set_mat4("proj", persp_proj, sun_shader);
  draw_model(cube, sun_shader);
}

void render_sphere() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  vec3 camera_pos = GLM_VEC3_ZERO_INIT;
  glm_translate(model, sphere_center);
  glm_scale(model, (vec3) {RADIUS, RADIUS, RADIUS});
  calc_cam_space(view);
  get_cam_loc(camera_pos);

  glUseProgram(test_shader);
  set_float("radius", RADIUS, test_shader);
  set_vec3("sphere_center", sphere_center, test_shader);
  set_vec3("camera_position", camera_pos, test_shader);
  set_vec3("light_pos", light_pos, test_shader);
  set_mat4("model", model, test_shader);
  set_mat4("view", view, test_shader);
  set_mat4("proj", persp_proj, test_shader);
  draw_model(sphere, test_shader);
}

void render_ocean() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  vec3 camera_pos = GLM_VEC3_ZERO_INIT;
  get_cam_loc(camera_pos);
  glm_rotate_x(model, glm_rad(90.0), model);
  glm_translate(model, sphere_center);
  glm_scale(model, (vec3) {
    RADIUS + ocean_offset,
    RADIUS + ocean_offset,
    RADIUS + ocean_offset
  });
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  calc_cam_space(view);
  glUseProgram(ocean_shader);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, normal_tex);
  set_int("norm_tex", 0, ocean_shader);
  set_vec3("light_pos", light_pos, ocean_shader);
  set_float("time", glfwGetTime(), ocean_shader);
  set_vec3("camera_position", camera_pos, ocean_shader);
  set_int("texture_enabled", normal_map_enabled, ocean_shader);
  set_mat4("model", model, ocean_shader);
  set_mat4("view", view, ocean_shader);
  set_mat4("proj", persp_proj, ocean_shader);
  draw_model(ocean, ocean_shader);
}

void render_normals() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  vec3 camera_pos = GLM_VEC3_ZERO_INIT;
  get_cam_loc(camera_pos);
  glm_translate(model, sphere_center);
  glm_scale(model, (vec3) {
    RADIUS + ocean_offset,
    RADIUS + ocean_offset,
    RADIUS + ocean_offset
  });
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  calc_cam_space(view);
  glUseProgram(normal_shader);
  set_mat4("model", model, normal_shader);
  set_mat4("view", view, normal_shader);
  set_mat4("proj", persp_proj, normal_shader);
  draw_model(ocean, normal_shader);
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
    int gs_len = strlen(gs);
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

void set_vec3(char *name, vec3 vector, unsigned int shader) {
  glUniform3fv(glGetUniformLocation(shader, name), 1,
                     (float *) vector);
}

void set_float(char *name, float fl, unsigned int shader) {
  glUniform1f(glGetUniformLocation(shader, name), fl);
}

void set_int(char *name, int i, unsigned int shader) {
  glUniform1i(glGetUniformLocation(shader, name), i);
}
