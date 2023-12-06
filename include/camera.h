#include <cglm/cglm.h>
#include <const.h>
#include <globals.h>

extern float RADIUS;

float CAM_HEIGHT = 0.5;

static vec3 base_up = { 0.0, 1.0, 0.0 };
static vec3 base_dir = { 0.0, 0.0, -1.0 };
static vec3 base_x = { 1.0, 0.0, 0.0 };

static float pitch = 0.0;
static float yaw = 0.0;

static vec3 cam_pos = { 0.0, 1.0, -5.0 };
static vec3 cam_right = { 1.0, 0.0, 0.0 };

static float cam_velocity;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

void calc_cam_space(mat4);
void get_cam_loc(vec3);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

float calc_displacement(vec2);
void calc_tex_coords(vec2, vec2);
