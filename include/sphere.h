#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sphere_str.h>

#define PI (3.1415926535)

int NUM_POINTS = 10000;
float EPSILON = 0.5;
double RADIUS = 1.0;

SPHERE *globe;

/* ------------- INTERNALLY DEFINED FUNCITONS -----------*/
void square_lattice();
void parameterize(LATTICE_POINT *);

/* ------------- EXTERNALLY DEFINED FUNCITONS -----------*/
