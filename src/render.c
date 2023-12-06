#include <render.h>

void init_scene() {
  update_shaders();
  sphere_mesh = gen_sphere();
  atmosphere = init_model(sphere_mesh);
  sphere = init_model(sphere_mesh);
  normal_tex = gen_texture("./maps/ocean_normal_map.jpg");
  ocean = init_model(sphere_mesh);
  sun = init_model(sphere_mesh);
  cube = gen_cube();
  min_max_height[0] = FLT_MAX;
  min_max_height[1] = FLT_MIN;
  refresh_sphere();

  glm_ortho(-1.0, 1.0, -1.0, 1.0, 0.0, 1000.0, ortho_proj);
  glm_perspective(glm_rad(45.0f), ((float) RES_X) / ((float) RES_Y), 0.1,
                  1000.0, persp_proj);

  glEnable(GL_PROGRAM_POINT_SIZE);
//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  imgui_add_i_slider("Resolution", &NUM_POINTS, 4, 10000);
  imgui_add_i_slider("Terrain Smoothing", &DEPTH, 0, 10);
  imgui_add_f_slider("Mountain Size", &incr_intv, 0.0, 0.1);
  imgui_add_f_slider("Continent Division", &FREQ, 0.0, 10.0);
  imgui_add_f_slider("Terrain Differential", &mountain_size, 1.0, 10.0);
  imgui_add_f_slider("Planet Radius", &RADIUS, 0.0, 100.0);
  imgui_add_i_slider("Camera Mode", &camera_mode, 0, 1);
  imgui_add_f_slider("Camera Speed", &speed, 0.0, 50.0);
  imgui_add_i_slider("Ocean Wave Mode", &normal_map_enabled, 0, 1);
  imgui_add_f_slider("Day Cycle Speed", &day_cycle, 0.1, 0.75);
  imgui_add_f_slider("Sea Level", &ocean_offset, 0.0, 0.05);
}

void update_shaders() {
  const char *v_shader = DIR"default.vs";
  const char *f_shader = DIR"default.fs";

  const char *o_shader = DIR"ocean.fs";
  const char *s_shader = DIR"sun.fs";
  const char *st_shader = DIR"star.fs";

  const char *v_shader_norm = DIR"norm_vec.vs";
  const char *g_shader = DIR"norm_vec.gs";
  const char *norm_frag_shader = DIR"norm_vec.fs";

  const char *a_shader = DIR"atmosphere.fs";

  test_shader = load_shader(v_shader, NULL, f_shader);
  ocean_shader = load_shader(v_shader, NULL, o_shader);
  sun_shader = load_shader(v_shader, NULL, s_shader);
  star_shader = load_shader(v_shader, NULL, st_shader);
  normal_shader = load_shader(v_shader_norm, g_shader, norm_frag_shader);
  atmosphere_shader = load_shader(v_shader, NULL, a_shader);
}

void render_scene(GLFWwindow *window) {
  float current_frame = glfwGetTime();
  delta_time = current_frame - last_frame;
  last_frame = current_frame;

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  render_sun();
  render_sphere();
  render_stars();
  render_ocean();
  render_atmosphere();
  //render_cube();
  //render_normals();
  render_imgui();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void render_sun() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 view = GLM_MAT4_IDENTITY_INIT;

  light_pos[0] = (RADIUS * 2.0) * (cos(day_cycle * glfwGetTime())) +
                 planet_pos[0];
  light_pos[1] = planet_pos[1];
  light_pos[2] = (RADIUS * 2.0) * (sin(day_cycle * glfwGetTime())) +
                 planet_pos[2];
  /*
  light_pos[0] = RADIUS * 2.0;
  light_pos[1] = planet_pos[1];
  light_pos[2] = 0.0;
  */

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
  glm_translate(model, planet_pos);
  glm_scale(model, (vec3) {RADIUS, RADIUS, RADIUS});
  calc_cam_space(view);
  get_cam_pos(camera_pos);

  glUseProgram(test_shader);
  set_float("radius", RADIUS, test_shader);
  set_vec2("range", min_max_height, test_shader);
  set_vec3("sphere_center", planet_pos, test_shader);
  set_vec3("camera_position", camera_pos, test_shader);
  set_vec3("light_pos", light_pos, test_shader);
  set_mat4("model", model, test_shader);
  set_mat4("view", view, test_shader);
  set_mat4("proj", persp_proj, test_shader);
  //draw_points(sphere);
  draw_model(sphere, test_shader);
}

void render_atmosphere() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  glm_translate(model, planet_pos);
  float scale = RADIUS * 1.25;
  glm_scale(model, (vec3) { scale, scale, scale });
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  calc_cam_space(view);

//  vec3 light_pos = { -RADIUS * 3.0, 0.0, 0.0 };
  vec3 cam_pos = GLM_VEC3_ZERO_INIT;
  get_cam_pos(cam_pos);

  glUseProgram(atmosphere_shader);
  set_mat4("model", model, atmosphere_shader);
  set_mat4("view", view, atmosphere_shader);
  set_mat4("proj", persp_proj, atmosphere_shader);
  set_vec3("light_pos", light_pos, atmosphere_shader);
  set_vec3("planet_pos", planet_pos, atmosphere_shader);
  set_vec3("view_pos", cam_pos, atmosphere_shader);
  draw_model(atmosphere, atmosphere_shader);
}

void render_stars() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  mat4 view = GLM_MAT4_IDENTITY_INIT;
  vec3 trans = GLM_VEC3_ZERO_INIT;
  glm_vec3_copy(planet_pos, trans);
  vec3 camera_pos = GLM_VEC3_ZERO_INIT;
  get_cam_pos(camera_pos);
  glm_vec3_add(trans, camera_pos, trans);

  glm_translate(model, trans);
  glm_scale(model, (vec3) {
    RADIUS + 100.0,
    RADIUS + 100.0,
    RADIUS + 100.0
  });
  calc_cam_space(view);

  glUseProgram(star_shader);
  set_float("time", glfwGetTime(), star_shader);
  set_mat4("model", model, star_shader);
  set_mat4("view", view, star_shader);
  set_mat4("proj", persp_proj, star_shader);
  draw_model(cube, star_shader);
}

void render_normals() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  vec3 camera_pos = GLM_VEC3_ZERO_INIT;
  get_cam_pos(camera_pos);
  glm_translate(model, planet_pos);
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

void render_ocean() {
  mat4 model = GLM_MAT4_IDENTITY_INIT;
  vec3 camera_pos = GLM_VEC3_ZERO_INIT;
  get_cam_pos(camera_pos);
  glm_rotate_x(model, glm_rad(90.0), model);
  glm_translate(model, planet_pos);
  glm_scale(model, (vec3) {
    RADIUS + (RADIUS * ocean_offset),
    RADIUS + (RADIUS * ocean_offset),
    RADIUS + (RADIUS * ocean_offset)
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

unsigned int load_shader(const char *v, const char *g, const char *f) {
  char *vert = NULL;
  char *geo = NULL;
  char *frag = NULL;
  FILE *vs = NULL;
  FILE *gs = NULL;
  FILE *fs = NULL;
  int handle = -1;
  if (v) {
    vert = (char *)(malloc(4096));
    int size = 0;
    vs = fopen(v, "r");
    while (fgetc(vs) != EOF) {
      size++;
    }
    fseek(vs, 0, SEEK_SET);
    if (!fread(vert, 1, (size_t) size, vs)) {
      printf("Issue reading vertex shader\n");
    }
    vert[size] = '\0';
    fclose(vs);
  }
  if (g) {
    geo = (char *)(malloc(4096));
    int size = 0;
    gs = fopen(g, "r");
    while (fgetc(gs) != EOF) {
      size++;
    }
    fseek(gs, 0, SEEK_SET);
    if (!fread(geo, 1, (size_t) size, gs)) {
      printf("Issue reading geometry shader\n");
    }
    geo[size] = '\0';
    fclose(gs);
  }
  if (f) {
    frag = (char *)(malloc(4096));
    int size = 0;
    fs = fopen(f, "r");
    while (fgetc(fs) != EOF) {
      size++;
    }
    fseek(fs, 0, SEEK_SET);
    if (!fread(frag, 1, (size_t) size, fs)) {
      printf("Issue reading fragment shader\n");
    }
    frag[size] = '\0';
    fclose(fs);
  }

  printf("%s, %s, %s\n", v, g, f);
  handle = init_shader(vert, geo, frag);
  if (vert) {
  free(vert);
  }
  if (geo) {
  free(geo);
  }
  if (frag) {
  free(frag);
  }
  return handle;
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

void set_vec2(char *name, vec2 vector, unsigned int shader) {
  glUniform2fv(glGetUniformLocation(shader, name), 1,
                     (float *) vector);
}
