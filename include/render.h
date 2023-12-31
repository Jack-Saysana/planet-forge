#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_wrapper.h>
#include <cglm/cglm.h>
#include <const.h>
#include <globals.h>
#include <model_str.h>

#define DIR "./shaders/"

extern int NUM_POINTS;
extern int DEPTH;
extern float FREQ;
extern float EPSILON;
extern float RADIUS;
extern float incr_intv;

float RES_X = RES_X_BASE;
float RES_Y = RES_Y_BASE;

unsigned int test_shader;
unsigned int atmosphere_shader;
unsigned int ocean_shader;
unsigned int normal_shader;
unsigned int sun_shader;
unsigned int star_shader;
unsigned int normal_tex;
MESH_DATA *sphere_mesh;
MESH_DATA *s_mesh;
MODEL *s_model;
MODEL *sphere;
MODEL *atmosphere;
MODEL *ocean;
MODEL *sun;
MODEL *cube;

mat4 ortho_proj;
mat4 persp_proj;

float day_cycle = 0.25;
float ocean_offset = 0.01;
int normal_map_enabled = 1;
extern float mountain_size;
vec2 min_max_height = GLM_VEC2_ZERO_INIT;
extern float mountain_size;
vec3 light_pos = GLM_VEC3_ZERO_INIT;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

unsigned int init_shader(const char *, const char *, const char *);
void test_render();
void render_sphere();
void render_atmosphere();
void render_ocean();
void render_sun();
void render_cube();
void render_normals();
void render_stars();
void set_float(char *, float, unsigned int);
void set_int(char *, int, unsigned int);
unsigned int load_shader(const char *, const char *, const char *);
void update_shaders();
void set_vec2(char *, vec2, unsigned int);
void set_mat4(char *, mat4, unsigned int);
void set_vec3(char *, vec3, unsigned int);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

MODEL *init_model(MESH_DATA *);
MESH_DATA *gen_sphere();
void draw_model(MODEL *, unsigned int);
void draw_points(MESH_DATA *);
void calc_cam_space(mat4);
void get_cam_pos(vec3);
MODEL *gen_cube();
unsigned int gen_texture(char *);
//void refresh_sphere();
