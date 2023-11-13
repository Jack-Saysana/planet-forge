#include <cglm/cglm.h>

typedef struct model {
  unsigned int num_indices;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
  unsigned int texture;
} MODEL;

typedef struct vertex {
  vec3 pos;
  vec3 norm;
  vec2 tex_pos;
} VERT;

typedef struct mesh_data {
  VERT *vertices;
  ivec3 *indices;
  size_t num_verts;
  size_t num_inds;
} MESH_DATA;

