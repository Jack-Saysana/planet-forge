#include <stdio.h>
#include <cglm/cglm.h>
#include <math.h>
#include <stdlib.h>
#include <sphere_str.h>

#define PI (3.1415926535)
#define X (0)
#define Y (1)
#define Z (2)

int NUM_POINTS = 10000;
int SEED = 1000;
int DEPTH = 3;
float FREQ = 0.05;

float EPSILON = 0.5;
double RADIUS = 1.0;

SPHERE *globe;

/* ------------- INTERNALLY DEFINED FUNCITONS -----------*/
void square_lattice();
void parameterize(LATTICE_POINT *);

/* ------------- EXTERNALLY DEFINED FUNCITONS -----------*/
float perlin(float, float, float, int, int);
