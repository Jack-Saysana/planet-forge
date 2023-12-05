#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <cglm/cglm.h>
#include <const.h>
#include <model_str.h>

int NUM_POINTS = 10000;
int SEED = 1000;
int DEPTH = 5;
float FREQ = 2.00;

float EPSILON = 0.5;
double RADIUS = 1.0;
float incr_intv = 0.0;
float mountain_size = 2.0;
extern vec2 min_max_height;
vec3 sphere_center;
extern int holding_zero;
extern int holding_nine;

/* ------------- INTERNALLY DEFINED FUNCITONS -----------*/

MESH_DATA *gen_sphere();
void deparameterize(MESH_DATA *);
int double_buffer(void **, size_t *, size_t);

/* ------------- EXTERNALLY DEFINED FUNCITONS -----------*/

float perlin(float, float, float, int, int);
size_t triangulate(vec3 *, size_t, ivec3 **);
MESH_DATA *gen_mesh(vec3 *, ivec3 *, size_t, size_t);
void init_model(MODEL *model, MESH_DATA *md);
