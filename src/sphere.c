#include <sphere.h>

void square_lattice() {
  /* Golden ratio */
  float phi = (1 + sqrt(5)) / 2; 
  LATTICE_POINT points[NUM_POINTS];
  for (int i = 0; i < NUM_POINTS; i++) {
    float x = ((float) i) / phi;
    /* Keep just the decimal part of x */
    float x_int = (int) x;
    x -= (float) x_int;
    points[i].x = x;
    points[i].y = (float) (i / ((float) (NUM_POINTS - 1)));
  }
  for (int i = 0; i < NUM_POINTS; i++) {
    printf("%d: (%f, %f)\n", i, points[i].x, points[i].y);
  }
}
