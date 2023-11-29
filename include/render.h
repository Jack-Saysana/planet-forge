#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <const.h>
#include <globals.h>
#include <shader_code.h>
#include <model_str.h>

float RES_X = RES_X_BASE;
float RES_Y = RES_Y_BASE;

unsigned int test_shader;
unsigned int ocean_shader;
unsigned int normal_shader;
unsigned int sun_shader;
unsigned int normal_tex; 
MESH_DATA *sphere_mesh;
MESH_DATA *s_mesh;
MODEL *s_model;
MODEL *sphere;
MODEL *ocean;
MODEL *sun;
MODEL *cube;

mat4 ortho_proj;
mat4 persp_proj;

extern double RADIUS;
float day_cycle = 0.25;
float ocean_offset = 0.01;
vec3 light_pos = GLM_VEC3_ZERO_INIT;
extern vec3 sphere_center;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

unsigned int init_shader(const char *, const char *, const char *);
void test_render();
void render_sphere();
void render_ocean();
void render_sun();
void render_cube();
void render_normals();
void set_mat4(char *, mat4, unsigned int);
void set_vec3(char *, vec3, unsigned int);
void set_float(char *, float, unsigned int);
void set_int(char *, int, unsigned int);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

MODEL *init_model(MESH_DATA *);
MESH_DATA *gen_sphere();
void draw_model(MODEL *, unsigned int);
void draw_points(MESH_DATA *);
void calc_cam_space(mat4);
void get_cam_loc(vec3);
MODEL *gen_cube();
void cheat();
unsigned int gen_texture(char *);
