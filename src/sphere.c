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
  for (int i = 0; i < NUM_POINTS; i++) {
    printf("%d: (%f, %f)\n", i, points[i].x, points[i].y);
  }
}
