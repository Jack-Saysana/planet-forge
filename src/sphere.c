#include <sphere.h>

void square_lattice() {
  /* Golden ratio */
  float phi = (1 + sqrt(5)) / 2; 
  LATTICE_POINT points[NUM_POINTS];
  for (float i = 0; i < (float) NUM_POINTS; i += 1.0) {
    float x = i / phi;
    /* Keep just the decimal part of x */
    float x_int = (int) x;
    x -= (float) x_int;
    points[(int) i].x = x;
    points[(int) i].y = (i + EPSILON) / ((float) (NUM_POINTS - 1) + (2.0 * EPSILON));
  }
  globe = (SPHERE *) malloc(sizeof(SPHERE) * NUM_POINTS);
  parameterize(points);
  for (int i = 0; i < NUM_POINTS; i++) {
    printf("%d: (%f, %f, %f)\n", i, globe[i].x, globe[i].y, globe[i].z);
  }
  free(globe);
}

void parameterize(LATTICE_POINT *points) {
  double halfpi = PI / 2.0;
  double twopi = PI * 2.0;
  for (int i = 0; i < NUM_POINTS; i++) {
    globe[i].x = RADIUS *
                 sin(((double) points[i].x) * halfpi) *
                 cos(((double) points[i].y) * twopi);
    globe[i].y = RADIUS *
                 sin(((double) points[i].x) * halfpi) *
                 sin(((double) points[i].y) * twopi);
    globe[i].z = RADIUS * cos(((double) points[i].x) * halfpi);
  }
}
