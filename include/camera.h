#include <cglm/cglm.h>
#include <const.h>

static vec3 base_up = { 0.0, 1.0, 0.0 };
static vec3 base_dir = { 0.0, 0.0, -1.0 };
static vec3 base_x = { 1.0, 0.0, 0.0 };

static float pitch = 0.0;
static float yaw = 0.0;

static vec3 cam_pos = GLM_VEC3_ZERO_INIT;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

void calc_cam_space(mat4);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================
