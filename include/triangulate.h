#include <cglm/cglm.h>
#include <const.h>

vec3 super_triangle_points[] = {
  SUPER_TRIANGLE_A,
  SUPER_TRIANGLE_B,
  SUPER_TRIANGLE_C
};

static vec3 NORM = { 0.0, 0.0, 1.0 };

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

int inside_circumcircle(vec3 *, size_t, ivec3, size_t);
int edge_in_triangle(ivec3, ivec2);
void get_point(vec3 *, size_t, size_t, vec3);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

int double_buffer(void **, size_t *, size_t);
