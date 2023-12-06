typedef struct quad_data {
  ivec3 tri;
  vec3 circum_center;
  float radius;
  size_t node;
  size_t next;
  size_t prev;
  /*
  size_t neighbors[3];
  size_t parent;
  size_t left;
  size_t right;
  int dim; // Represents the dimension by which child nodes are sorted
  */
} Q_DATA;

typedef struct quad_node {
  // Index of first child node in quad tree's "nodes" buffer. Following three
  // children are adjacent
  size_t first_child;
  // Index of first data element stored at this quad tree node in the quad
  // tree's "data" buffer
  size_t data_head;
  size_t data_tail;
  vec4 extents;
} Q_NODE;

typedef struct quad_tree {
  Q_NODE *nodes;
  Q_DATA *data;
  size_t node_buf_len;
  size_t node_buf_size;
  size_t data_buf_len;
  size_t data_buf_size;
  size_t max_depth;
} Q_TREE;
