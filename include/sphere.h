#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cglm/cglm.h>
#include <const.h>
#include <model_str.h>

int NUM_POINTS = NUM_POINTS_INIT;
int SEED = 1000;
int DEPTH = DEPTH_INIT;
float FREQ = FREQ_INIT;

float EPSILON = EPSILON_INIT;
float RADIUS = RADIUS_INIT;

extern MESH_DATA *sphere_mesh;
extern MODEL *sphere;

/* ------------- INTERNALLY DEFINED FUNCITONS -----------*/

MESH_DATA *gen_sphere();
void deparameterize(MESH_DATA *);
int double_buffer(void **, size_t *, size_t);
float calc_displacement(vec2);
void calc_tex_coords(vec2, vec2);

/* ------------- EXTERNALLY DEFINED FUNCITONS -----------*/

float perlin(float, float, float, int, int);
size_t triangulate(vec3 *, size_t, ivec3 **);
MESH_DATA *gen_mesh(vec3 *, ivec3 *, size_t, size_t);
MODEL *init_model(MESH_DATA *md);
void free_mesh_data(MESH_DATA *);
void free_model(MODEL *);
void refresh_sphere();
