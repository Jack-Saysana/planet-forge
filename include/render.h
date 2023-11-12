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
MODEL test_model;
MESH_DATA *sphere;

mat4 ortho_proj;
mat4 persp_proj;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

unsigned int init_shader(const char *, const char *, const char *);
void test_render();
void render_sphere();
void set_mat4(char *, mat4, unsigned int);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

void init_model(MODEL *, MESH_DATA *);
MESH_DATA *gen_sphere();
void draw_model(MODEL *, unsigned int);
void draw_points(MESH_DATA *);
void calc_cam_space(mat4);
