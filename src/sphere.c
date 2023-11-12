#include <sphere.h>

MESH_DATA *gen_sphere() {
  /* Golden ratio */
  float phi = (1 + sqrt(5)) / 2;
  VERT *points = malloc(sizeof(VERT) * NUM_POINTS * 2);
  for (float i = 0; i < (float) NUM_POINTS; i += 1.0) {
    int index = (int) i;
    float x = i / phi;
    /* Keep just the decimal part of x */
    float x_int = (int) x;
    x -= (float) x_int;
    points[index].pos[X] = x;
    points[index].pos[Y] = (i + EPSILON) / ((float) (NUM_POINTS - 1) +
                         (2.0 * EPSILON));
    points[index].pos[Z] = 0.0;

    // TODO normals will have to be recalculated upon triangulation
    glm_vec3_zero(points[index].norm);
    glm_vec2_copy(points[index].pos, points[index].tex_pos);
  }

  // TODO Generate edges here

  deparameterize(points);

  MESH_DATA *sphere = malloc(sizeof(MESH_DATA));
  if (sphere == NULL) {
    return NULL;
  }
  sphere->vertices = points;
  sphere->indices = NULL; // TODO correct after triangulation
  sphere->num_verts = NUM_POINTS * 2;
  sphere->num_inds = 0;
  return sphere;
}

void deparameterize(VERT *points) {
  vec3 cur_sphere_pt = GLM_VEC3_ZERO_INIT;
  for (int i = 0; i < NUM_POINTS; i++) {
    // deparameterize into a sphere
    float theta = 2.0 * PI * points[i].pos[X];
    float phi = acos(1.0 - (2.0 * points[i].pos[Y]));
    cur_sphere_pt[X] = RADIUS * cos(theta) * sin(phi);
    cur_sphere_pt[Y] = RADIUS * sin(theta) * sin(phi);
    cur_sphere_pt[Z] = RADIUS * cos(phi);

    // Apply perlin noise displacement
    /*
    vec3 disp_vector = GLM_VEC3_ZERO_INIT;
    glm_vec3_copy(cur_sphere_pt, disp_vector);
    glm_normalize(disp_vector);
    float displacement = perlin(points[i].pos[X], points[i].pos[Y], FREQ,
                                DEPTH, SEED);
    disp_vector[X] *= displacement;
    disp_vector[Y] *= displacement;
    disp_vector[Z] *= displacement;
    glm_vec3_add(cur_sphere_pt, disp_vector, cur_sphere_pt);
    */

    glm_vec3_copy(cur_sphere_pt, points[i].pos);
  }
}
