#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cglm/cglm.h>
#include <const.h>
#include <model_str.h>

int NUM_POINTS = 10000;
int SEED = 1000;
int DEPTH = 3;
float FREQ = 0.05;

float EPSILON = 0.5;
double RADIUS = 1.0;

/* ------------- INTERNALLY DEFINED FUNCITONS -----------*/
MESH_DATA *gen_sphere();
void deparameterize(VERT *);

/* ------------- EXTERNALLY DEFINED FUNCITONS -----------*/
float perlin(float, float, float, int, int);
