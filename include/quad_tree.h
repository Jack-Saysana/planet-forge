#include <cglm/cglm.h>
#include <const.h>
#include <quad_tree_str.h>

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

Q_TREE *init_quad_tree(vec4, size_t);
void reset_tree(Q_TREE *, vec4, size_t);
void insert_triangle(Q_TREE *, Q_DATA);
Q_DATA remove_triangle(Q_TREE *, Q_DATA);
size_t search_tree(Q_TREE *, vec2, Q_DATA **);
void read_buffer(Q_TREE *, Q_NODE *, Q_DATA **, size_t *, size_t *);
void read_all_children(Q_TREE *, Q_NODE *, Q_DATA **, size_t *, size_t *);
void append_to_node(Q_TREE *, size_t, size_t);
size_t search_node(Q_TREE *, size_t, vec3, float);
int detect_quad(Q_NODE *, Q_DATA *);
int detect_quad_pt(Q_NODE *, vec2);
size_t get_child_node(Q_TREE *, size_t, unsigned int);

int circle_aabb_collision(vec2, float, vec2, float, float);
int point_aabb_collision(vec2, vec2, float, float);

void print_tree(Q_TREE *, size_t);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

int double_buffer(void **, size_t *, size_t);
