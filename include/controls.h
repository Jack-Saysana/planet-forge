#include <stdio.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <const.h>
#include <globals.h>

extern float RES_X;
extern float RES_Y;

int first_move = 1;
double prev_x = 0.0;
double prev_y = 0.0;

int cursor_enabled = 1;

int holding_grave = 0;

extern mat4 ortho_proj;
extern mat4 persp_proj;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================


// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

void cam_rotate_x(float);
void cam_rotate_y(float);
void cam_translate(vec3);
