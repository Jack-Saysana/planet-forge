#include <sphere.h>

MESH_DATA *gen_sphere() {
  /* Golden ratio */
  float phi = (1 + sqrt(5)) / 2;
  vec3 *points = malloc(sizeof(vec3) * NUM_POINTS);
  glm_vec3_copy((vec3) {0.0, 0.0, 0.0}, points[0]);
  glm_vec3_copy((vec3) {1.0, 0.0, 0.0}, points[1]);
  glm_vec3_copy((vec3) {1.0, 1.0, 0.0}, points[2]);
  glm_vec3_copy((vec3) {0.0, 1.0, 0.0}, points[3]);
  glm_vec3_copy((vec3) {0.0, 0.0, -5.0}, sphere_center);

  for (float i = 4; i < (float) NUM_POINTS; i += 1.0) {
    int index = (int) i;
    float x = i / phi;
    /* Keep just the decimal part of x */
    float x_int = (int) x;
    x -= (float) x_int;
    if (x < 0.005) {
      x = 0.0;
    } else if (x > 0.995) {
      x = 1.0;
    }
    points[index][X] = x;
    points[index][Y] = (i + EPSILON) / ((float) (NUM_POINTS - 1) +
                       (2.0 * EPSILON));
    if (points[index][Y] < 0.005) {
      points[index][Y] = 0.0;
    } else if (points[index][Y] > 0.995) {
      points[index][Y] = 1.0;
    }

    points[index][Z] = 0.0;
  }

  // Triangulate list of points
  ivec3 *triangles = NULL;
  size_t num_triangles = triangulate(points, NUM_POINTS, &triangles);

  MESH_DATA *mesh = gen_mesh(points, triangles, NUM_POINTS, num_triangles);
  free(points);
  free(triangles);

  // Convert flat mesh into a sphere
  deparameterize(mesh);
  return mesh;
}

void deparameterize(MESH_DATA *mesh) {
  vec3 cur_sphere_pt = GLM_VEC3_ZERO_INIT;
  for (size_t i = 0; i < mesh->num_verts; i++) {
    float theta = 2.0 * PI * mesh->vertices[i].pos[X];
    float phi = acos(1.0 - (2.0 * mesh->vertices[i].pos[Y]));
    cur_sphere_pt[X] = cos(theta) * sin(phi);
    cur_sphere_pt[Y] = sin(theta) * sin(phi);
    cur_sphere_pt[Z] = cos(phi);

    glm_vec3_copy(cur_sphere_pt, mesh->vertices[i].pos);
    glm_vec3_copy(cur_sphere_pt, mesh->vertices[i].norm);
    glm_vec3_normalize(mesh->vertices[i].norm);
  }
}

void increase_height(MESH_DATA *mesh) {
  if (holding_zero) {
    incr_intv += 0.0001;
  }
  VERT *vertex;
  for (size_t i = 0; i < mesh->num_verts; i++) {
    vertex = mesh->vertices + i;
    float displacement = glm_vec3_distance((vec3) {0.0, 0.0, 0.0}, vertex->pos) - 1.0;
    if (displacement > 0.035) {
      vec3 disp_vector = GLM_VEC3_ZERO_INIT;
      glm_vec3_copy(vertex->pos, disp_vector);
      glm_vec3_normalize(disp_vector);
      glm_vec3_scale(disp_vector, (1.0 + displacement - 0.035) * incr_intv, disp_vector);
      glm_vec3_add(disp_vector, vertex->pos, vertex->pos);
    }
  }
}

void decrease_height(MESH_DATA *mesh) {
  if (holding_nine) {
    incr_intv -= 0.0001;
    VERT *vertex;
    for (size_t i = 0; i < mesh->num_verts; i++) {
      vertex = mesh->vertices + i;
      float displacement = glm_vec3_distance((vec3) {0.0, 0.0, 0.0}, vertex->pos) - 1.0;
      if (displacement > 0.035) {
        vec3 disp_vector = GLM_VEC3_ZERO_INIT;
        glm_vec3_copy(vertex->pos, disp_vector);
        glm_vec3_normalize(disp_vector);
        glm_vec3_scale(disp_vector, (displacement - 0.035) * incr_intv, disp_vector);
        glm_vec3_sub(vertex->pos, disp_vector, vertex->pos);
      }
    }
  }
}

void apply_noise(MESH_DATA *mesh) {
  float offset = 1.0;
  vec3 cur_sphere_pt = GLM_VEC3_ZERO_INIT;
  for (size_t i = 0; i < mesh->num_verts; i++) {
    glm_vec3_copy(mesh->vertices[i].pos, cur_sphere_pt);

    // Apply perlin noise displacement
    vec3 disp_vector = GLM_VEC3_ZERO_INIT;
    glm_vec3_copy(cur_sphere_pt, disp_vector);
    glm_normalize(disp_vector);

    // Addresses seam issues from perlin noise
    float uvx = cos(PI * (mesh->vertices[i].tex_pos[X] -
                          mesh->vertices[i].tex_pos[Y]));
    float uvy = cos(PI * (mesh->vertices[i].tex_pos[X] +
                    mesh->vertices[i].tex_pos[Y] - 1.0));
    float mask = fmax((uvx + uvy) / 10.0, 0.0);
    float displacement = perlin(mesh->vertices[i].tex_pos[X] * offset,
                                mesh->vertices[i].tex_pos[Y] * offset, FREQ,
                                DEPTH, SEED);

    // Clamp perlin output between -0.5 and 0.5
    displacement -= 0.5;
    //displacement *= (float) RADIUS;
    disp_vector[X] *= displacement * mask;
    disp_vector[Y] *= displacement * mask;
    disp_vector[Z] *= displacement * mask;
    glm_vec3_copy(cur_sphere_pt, mesh->vertices[i].pos);
    glm_vec3_add(disp_vector, mesh->vertices[i].pos, mesh->vertices[i].pos);
  }

  // Calculate normal of each triangle
  ivec3 cur_triangle = { 0, 0, 0 };
  for (size_t i = 0; i < mesh->num_inds; i++) {
    glm_ivec3_copy(mesh->indices[i], cur_triangle);

    vec3 ab = GLM_VEC3_ZERO_INIT;
    vec3 ac = GLM_VEC3_ZERO_INIT;
    vec3 norm = GLM_VEC3_ZERO_INIT;
    glm_vec3_sub(mesh->vertices[cur_triangle[B]].pos,
                 mesh->vertices[cur_triangle[A]].pos,
                 ab);
    glm_vec3_sub(mesh->vertices[cur_triangle[C]].pos,
                 mesh->vertices[cur_triangle[A]].pos,
                 ac);

    glm_vec3_cross(ab, ac, norm);
    glm_vec3_normalize(norm);

    glm_vec3_copy(norm, mesh->vertices[cur_triangle[A]].norm);
    glm_vec3_copy(norm, mesh->vertices[cur_triangle[B]].norm);
    glm_vec3_copy(norm, mesh->vertices[cur_triangle[C]].norm);
  }
}

int double_buffer(void **buffer, size_t *buf_size, size_t unit_size) {
  void *new_buf = realloc(*buffer, 2 * (*buf_size) * unit_size);
  if (new_buf == NULL) {
    return -1;
  }
  (*buffer) = new_buf;
  *buf_size = 2 * *buf_size;
  return 0;
}
