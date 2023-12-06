#include <cglm/cglm.h>
#include <const.h>
#include <quad_tree_str.h>

vec3 super_triangle_points[] = {
  SUPER_TRIANGLE_A,
  SUPER_TRIANGLE_B,
  SUPER_TRIANGLE_C
};

static vec3 NORM = { 0.0, 0.0, 1.0 };

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

//int inside_circumcircle(vec3 *, size_t, ivec3, size_t);
int inside_circumcircle(vec3 *, size_t, Q_DATA, size_t);
int edge_in_triangle(ivec3, ivec2);
void get_point(vec3 *, size_t, size_t, vec3);
void calc_circumcenter(vec3 *, size_t, ivec3, vec3);
Q_DATA calc_tri_data(vec3 *, size_t, ivec3);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

Q_TREE *init_quad_tree(vec4, size_t);
void reset_tree(Q_TREE *, vec4, size_t);
void insert_triangle(Q_TREE *, Q_DATA);
Q_DATA remove_triangle(Q_TREE *, Q_DATA);
size_t search_tree(Q_TREE *, vec2, Q_DATA **);

int double_buffer(void **, size_t *, size_t);
