#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cglm/cglm.h>
#include <const.h>
#include <model_str.h>

#define NUM_POINTS_INIT (4)
#define DEPTH_INIT (5)
#define FREQ_INIT (0.05)
#define EPSILON_INIT (0.5)
#define RADIUS_INIT (1.0)

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

/* ------------- EXTERNALLY DEFINED FUNCITONS -----------*/

float perlin(float, float, float, int, int);
size_t triangulate(vec3 *, size_t, ivec3 **);
MESH_DATA *gen_mesh(vec3 *, ivec3 *, size_t, size_t);
MODEL *init_model(MESH_DATA *md);
void free_mesh_data(MESH_DATA *);
void free_model(MODEL *);
void refresh_sphere();
