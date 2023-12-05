#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_wrapper.h>
#include <cglm/cglm.h>
#include <const.h>
#include <globals.h>
#include <shader_code.h>
#include <model_str.h>

extern int NUM_POINTS;
extern int DEPTH;
extern float FREQ;
extern float EPSILON;
extern float RADIUS;

float RES_X = RES_X_BASE;
float RES_Y = RES_Y_BASE;

unsigned int test_shader;
unsigned int atmosphere_shader;
MESH_DATA *sphere_mesh;
MODEL *sphere;
MODEL *atmosphere;

mat4 ortho_proj;
mat4 persp_proj;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

unsigned int init_shader(const char *, const char *, const char *);
void test_render();
void render_sphere();
void render_atmosphere();
void set_mat4(char *, mat4, unsigned int);
void set_vec3(char *, vec3, unsigned int);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

MODEL *init_model(MESH_DATA *);
MESH_DATA *gen_sphere();
void draw_model(MODEL *, unsigned int);
void draw_points(MESH_DATA *);
void calc_cam_space(mat4);
void get_cam_pos(vec3);
