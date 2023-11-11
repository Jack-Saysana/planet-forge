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
    points[(int) i].coords[X] = x;
    points[(int) i].coords[Y] = (i + EPSILON) / ((float) (NUM_POINTS - 1) + (2.0 * EPSILON));
  }
  globe = (SPHERE *) malloc(sizeof(SPHERE) * NUM_POINTS);
  parameterize(points);
  /*
  for (int i = 0; i < NUM_POINTS; i++) {
    printf("%d: (%f, %f, %f)\n", i, globe[i].coords[X], 
                                    globe[i].coords[Y],
                                    globe[i].coords[Z]);
  }
  */
  free(globe);
}

void parameterize(LATTICE_POINT *points) {
  double halfpi = PI / 2.0;
  double twopi = PI * 2.0;
  for (int i = 0; i < NUM_POINTS; i++) {
    /* Parameterize into a sphere */
    globe[i].coords[X] = (float) (RADIUS *
                  sin(((double) points[i].coords[X]) * halfpi) *
                  cos(((double) points[i].coords[Y]) * twopi));
    globe[i].coords[Y] = (float) (RADIUS *
                 sin(((double) points[i].coords[X]) * halfpi) *
                 sin(((double) points[i].coords[Y]) * twopi));
    globe[i].coords[Z] = (float) (RADIUS * cos(((double) points[i].coords[X]) * halfpi));
    /* Apply perlin noise displacement */
    vec3 disp_vector = GLM_VEC3_ZERO_INIT;
    glm_vec3_copy(globe[i].coords, disp_vector);
    glm_normalize(disp_vector);
    float displacement = perlin(points[i].coords[X], points[i].coords[Y], FREQ, DEPTH, SEED);
    disp_vector[X] *= displacement;
    disp_vector[Y] *= displacement;
    globe[i].coords[X] += disp_vector[X];
    globe[i].coords[Y] += disp_vector[Y];
  }
}
