#include <triangulate.h>

/*
  Implementation of the Bowyer-Watson Algorithm for creating a Delauny
  Triangulation
  Args:
  - vec3 *points: Points to triangulate
  - size_t num_points: Number of points in points
  - ivec3 **dest: Pointer to buffer to populate with triangles

  Returns: Number of triangles created
*/
size_t triangulate(vec3 *points, size_t num_points, ivec3 **dest) {
  /*
  ivec3 *triangles = malloc(sizeof(ivec3) * STARTING_BUFF_LEN);
  if (triangles == NULL) {
    fprintf(stderr, "sphere.c: Unable to reallocate triangle buffer\n");
    exit(1);
  }
  size_t t_buff_len = 0;
  size_t t_buff_size = STARTING_BUFF_LEN;
  */
  vec4 extents = GLM_VEC4_ZERO_INIT;
  /*
  extents[X_MIN] = -16.0;
  extents[X_MAX] =  16.0;
  extents[Y_MIN] = -16.0;
  extents[Y_MAX] =  16.0;
  */
  extents[X_MIN] = -1.0;
  extents[X_MAX] =  1.0;
  extents[Y_MIN] = -1.0;
  extents[Y_MAX] =  1.0;

  Q_TREE *triangles = init_quad_tree(extents, TRIANGLE_TREE_MAX_DEPTH);

  //ivec3 *bad_triangles = malloc(sizeof(ivec3) * STARTING_BUFF_LEN);
  Q_DATA *bad_triangles = malloc(sizeof(Q_DATA) * STARTING_BUFF_LEN);
  if (bad_triangles == NULL) {
    fprintf(stderr, "sphere.c: Unable to reallocate bad triangle buffer\n");
    exit(1);
  }
  size_t bt_buff_len = 0;
  size_t bt_buff_size = STARTING_BUFF_LEN;

  ivec2 *polygon = malloc(sizeof(ivec2) * STARTING_BUFF_LEN);
  if (polygon == NULL) {
    fprintf(stderr, "sphere.c: Unable to reallocate polygon buffer\n");
    exit(1);
  }
  size_t p_buff_len = 0;
  size_t p_buff_size = STARTING_BUFF_LEN;

  Q_DATA *search_result = NULL;
  size_t search_res_len = 0;

  // The initial triangulation contains only the super triangle.
  // Super triangle indices extend past the highest index of "points", so we
  // can differentiate real points from super triangle points
  /*
  glm_ivec3_copy((ivec3) { num_points + A, num_points + B, num_points + C },
                 triangles[0]);
  t_buff_len++;
  */
  insert_triangle(triangles, calc_tri_data(points, num_points,
                  (ivec3) { num_points + A, num_points + B, num_points + C }));

  int status = 0;
  for (size_t cur_point = 0; cur_point < num_points; cur_point++) {
    //if (cur_point == 91) {
    /*
    if (cur_point == 90) {
      fprintf(stderr, "sdada\n");
    }
    */
    // Check which current triangles are invalidated due to new point
    /*
    bt_buff_len = 0;
    for (size_t cur_tri = 0; cur_tri < t_buff_len; cur_tri++) {
      if (inside_circumcircle(points, num_points, triangles[cur_tri],
                              cur_point)) {
        glm_ivec3_copy(triangles[cur_tri], bad_triangles[bt_buff_len]);

        bt_buff_len++;
        if (bt_buff_len == bt_buff_size) {
          status = double_buffer((void **) &bad_triangles, &bt_buff_size,
                                 sizeof(ivec3));
          if (status) {
            fprintf(stderr, "sphere.c: Unable to reallocate bad triangles\n");
            exit(1);
          }
        }
      }
    }
    */
    search_res_len = search_tree(triangles, (vec2) { points[cur_point][X],
                                 points[cur_point][Y] }, &search_result);
    bt_buff_len = 0;
    for (size_t cur_tri = 0; cur_tri < search_res_len; cur_tri++) {
      //if (search_result[cur_tri].tri[0] == 91 && search_result[cur_tri].tri[1] == 89 &&
      //    search_result[cur_tri].tri[2] == 34) {
      /*
      if (search_result[cur_tri].tri[0] == 90 && search_result[cur_tri].tri[1] == 89 &&
          search_result[cur_tri].tri[2] == 34) {
        printf("fjfjfjfjfjfjf\n");
      }
      */
      if (inside_circumcircle(points, num_points, search_result[cur_tri],
                              cur_point)) {
        //glm_ivec3_copy(search_result[cur_tri].tri, bad_triangles[bt_buff_len]);
        bad_triangles[bt_buff_len] = search_result[cur_tri];

        bt_buff_len++;
        if (bt_buff_len == bt_buff_size) {
          //status = double_buffer((void **) &bad_triangles, &bt_buff_size,
          //                       sizeof(ivec3));
          status = double_buffer((void **) &bad_triangles, &bt_buff_size,
                                 sizeof(Q_DATA));
          if (status) {
            fprintf(stderr, "sphere.c: Unable to reallocate bad triangles\n");
            exit(1);
          }
        }
      }
    }
    free(search_result);
    search_res_len = 0;

    // Construct the polygon formed by the invalid triangles
    p_buff_len = 0;
    for (size_t cur_bad_tri = 0; cur_bad_tri < bt_buff_len; cur_bad_tri++) {
      ivec3 cur_bt = { 0, 0, 0 };
      //glm_ivec3_copy(bad_triangles[cur_bad_tri], cur_bt);
      glm_ivec3_copy(bad_triangles[cur_bad_tri].tri, cur_bt);

      ivec2 cur_edge = { 0, 0 };
      // Iterate over all edges in current triangle
      for (int i = 0; i < 3; i++) {
        cur_edge[0] = cur_bt[i];
        if (i == 2) {
          cur_edge[1] = cur_bt[0];
        } else {
          cur_edge[1] = cur_bt[i + 1];
        }

        // Check if edge is unique in list of bad triangles
        int unique = 1;
        for (size_t i = 0; i < bt_buff_len; i++) {
          if (i != cur_bad_tri &&
              //edge_in_triangle(bad_triangles[i], cur_edge)) {
              edge_in_triangle(bad_triangles[i].tri, cur_edge)) {
            unique = 0;
            break;
          }
        }

        // Add edge to polygon if its unique
        if (unique) {
          glm_ivec2_copy(cur_edge, polygon[p_buff_len]);
          p_buff_len++;
          if (p_buff_len == p_buff_size) {
            status = double_buffer((void **) &polygon, &p_buff_size,
                                   sizeof(ivec2));
            if (status) {
              fprintf(stderr, "sphere.c: Unable to reallocate polygon\n");
              exit(1);
            }
          }
        }
      }
    }

    for (size_t cur_bad_tri = 0; cur_bad_tri < bt_buff_len; cur_bad_tri++) {
      /*
      ivec3 bad_tri = { 0, 0, 0 };
      glm_ivec3_copy(bad_triangles[cur_bad_tri], bad_tri);

      // Remove invalid triangle from triangulation
      for (size_t i = 0; i < t_buff_len; i++) {
        if (triangles[i][A] == bad_tri[A] &&
            triangles[i][B] == bad_tri[B] &&
            triangles[i][C] == bad_tri[C]) {
          t_buff_len--;
          glm_ivec3_copy(triangles[t_buff_len], triangles[i]);
          i--;
        }
      }
      */
      /*
      if (cur_bad_tri == 0) {
        fprintf(stderr, "%ld: %ld\n", cur_point, cur_bad_tri);
      }
      */
      remove_triangle(triangles, bad_triangles[cur_bad_tri]);
    }

    // Create new triangles from new point
    for (size_t cur_edge = 0; cur_edge < p_buff_len; cur_edge++) {
      ivec2 e1 = { cur_point, polygon[cur_edge][0] };
      ivec2 e2 = { cur_point, polygon[cur_edge][1] };

      vec3 pt1 = GLM_VEC3_ZERO_INIT;
      vec3 pt2 = GLM_VEC3_ZERO_INIT;
      vec3 edge1 = GLM_VEC3_ZERO_INIT;
      vec3 edge2 = GLM_VEC3_ZERO_INIT;

      get_point(points, num_points, e1[1], pt1);
      get_point(points, num_points, e1[0], pt2);
      glm_vec3_sub(pt1, pt2, edge1);

      get_point(points, num_points, e2[1], pt1);
      get_point(points, num_points, e2[0], pt2);
      glm_vec3_sub(pt1, pt2, edge2);

      vec3 e1_cross_e2 = GLM_VEC3_ZERO_INIT;
      glm_vec3_cross(edge1, edge2, e1_cross_e2);

      // Ensure all new triangles have counter-clockwise winding order
      ivec3 new_tri = { 0, 0, 0 };
      if (glm_vec3_dot(e1_cross_e2, NORM) > 0.0) {
        new_tri[A] = cur_point;
        new_tri[B] = polygon[cur_edge][0];
        new_tri[C] = polygon[cur_edge][1];
      } else {
        new_tri[A] = cur_point;
        new_tri[B] = polygon[cur_edge][1];
        new_tri[C] = polygon[cur_edge][0];
      }

      /*
      glm_ivec3_copy(new_tri, triangles[t_buff_len]);
      t_buff_len++;
      if(t_buff_len == t_buff_size) {
        status = double_buffer((void **) &triangles, &t_buff_size,
                               sizeof(ivec3));
        if (status) {
          fprintf(stderr, "sphere.c: Unable to reallocate triangles\n");
          exit(1);
        }
      }
      */
      //if (new_tri[0] == 91 && new_tri[1] == 89 && new_tri[2] == 34) {
      /*
      if (new_tri[0] == 90 && new_tri[1] == 89 && new_tri[2] == 34) {
        fprintf(stderr, "INS!\n");
      }
      */
      Q_DATA to_insert = calc_tri_data(points, num_points, new_tri);
      insert_triangle(triangles, to_insert);
    }
  }

  size_t t_buff_len = triangles->data_buf_len;
  ivec3 *result = malloc(sizeof(ivec3) * triangles->data_buf_len);
  if (result == NULL) {
    fprintf(stderr,
            "triangulate.c: Unable to allocate triangle result buffer\n");
    exit(1);
  }
  for (size_t i = 0; i < t_buff_len; i++) {
    glm_ivec3_copy(triangles->data[i].tri, result[i]);
  }

  // Remove all triangles with super triangle points
  /*
  for (size_t cur_tri = 0; cur_tri < t_buff_len; cur_tri++) {
    ivec3 tri = { 0, 0, 0 };
    glm_ivec3_copy(triangles[cur_tri], tri);
    if (tri[A] >= num_points || tri[B] >= num_points || tri[C] >= num_points) {
      t_buff_len--;
      glm_ivec3_copy(triangles[t_buff_len], triangles[cur_tri]);
      cur_tri--;
    }
  }
  */
  //FILE *out = fopen("out.txt", "w");
  for (size_t cur_tri = 0; cur_tri < t_buff_len; cur_tri++) {
    ivec3 tri = { 0, 0, 0 };
    glm_ivec3_copy(result[cur_tri], tri);
    if (tri[A] >= num_points || tri[B] >= num_points || tri[C] >= num_points) {
      t_buff_len--;
      glm_ivec3_copy(result[t_buff_len], result[cur_tri]);
      cur_tri--;
    }
      /*
    } else {
      vec3 first, second, third;
      get_point(points, num_points, result[cur_tri][A], first);
      get_point(points, num_points, result[cur_tri][B], second);
      get_point(points, num_points, result[cur_tri][C], third);

      char buff[1024] = "";
      char index_buff[1024] = "";
      size_t buff_len = 0;
      size_t val = cur_tri;
      while (val) {
        buff[buff_len] = '0' + (val % 10);
        buff[buff_len + 1] = '_';
        buff_len += 2;
        val /= 10;
      }
      for (int j = buff_len - 1; j >= 0; j--) {
        index_buff[buff_len - 1 - j] = buff[j];
      }

      if (result[cur_tri][A] == 34 || result[cur_tri][B] == 34 ||
          result[cur_tri][C] == 34) {
        fprintf(out, "(%d, %d, %d)\n", result[cur_tri][A], result[cur_tri][B],
                result[cur_tri][C]);
        fprintf(out, "A%s = ", index_buff);
        fprintf(out, "( %.4f, %.4f )\n", first[0], first[1]);
        fprintf(out, "B%s = ", index_buff);
        fprintf(out, "( %.4f, %.4f )\n", second[0], second[1]);
        fprintf(out, "C%s = ", index_buff);
        fprintf(out, "( %.4f, %.4f )\n", third[0], third[1]);
      }
    }
    */
  }
  //fclose(out);

  free(bad_triangles);
  free(polygon);

  //*dest = triangles;
  *dest = result;

  return t_buff_len;
}

int edge_in_triangle(ivec3 triangle, ivec2 edge) {
  ivec2 cur_edge = { 0, 0 };
  for (int i = 0; i < 3; i++) {
    cur_edge[0] = triangle[i];
    if (i == 2) {
      cur_edge[1] = triangle[0];
    } else {
      cur_edge[1] = triangle[i + 1];
    }

    if ((cur_edge[0] == edge[0] && cur_edge[1] == edge[1]) ||
        (cur_edge[0] == edge[1] && cur_edge[1] == edge[0])) {
      return 1;
    }
  }
  return 0;
}

/*
  Check if a point is inside the circumcircle of a triangle
  Method utilizes the deteminite of a triangle.
  More info:
  https://en.wikipedia.org/wiki/Delaunay_triangulation#Algorithms

  Args:
  - vec3 *points: List of points
  - ivec3 triangle: List of 3 indices of points which form the triangle to be
    tested (Assumed points are sorted in counter-clockwise order
  - int point: Index of point in points to be tested
*/
//int inside_circumcircle(vec3 *points, size_t num_points, ivec3 triangle,
//                        size_t point) {
int inside_circumcircle(vec3 *points, size_t num_points, Q_DATA triangle,
                        size_t point) {
  vec3 a = GLM_VEC3_ZERO_INIT;
  vec3 b = GLM_VEC3_ZERO_INIT;
  vec3 c = GLM_VEC3_ZERO_INIT;
  vec3 d = GLM_VEC3_ZERO_INIT;
  get_point(points, num_points, triangle.tri[A], a);
  get_point(points, num_points, triangle.tri[B], b);
  get_point(points, num_points, triangle.tri[C], c);
  get_point(points, num_points, point, d);

  vec3 a_min_d = GLM_VEC3_ZERO_INIT;
  vec3 b_min_d = GLM_VEC3_ZERO_INIT;
  vec3 c_min_d = GLM_VEC3_ZERO_INIT;
  glm_vec3_sub(a, d, a_min_d);
  glm_vec3_sub(b, d, b_min_d);
  glm_vec3_sub(c, d, c_min_d);

  mat3 test_mat = {
   {a_min_d[X], b_min_d[X], c_min_d[X]},
   {a_min_d[Y], b_min_d[Y], c_min_d[Y]},
   {
     (a_min_d[X]*a_min_d[X]) + (a_min_d[Y]*a_min_d[Y]),
     (b_min_d[X]*b_min_d[X]) + (b_min_d[Y]*b_min_d[Y]),
     (c_min_d[X]*c_min_d[X]) + (c_min_d[Y]*c_min_d[Y]),
   }
  };

  return glm_mat3_det(test_mat) > 0.0;

  /*
  vec3 a = GLM_VEC3_ZERO_INIT;
  vec3 d = GLM_VEC3_ZERO_INIT;
  get_point(points, num_points, triangle[A], a);
  get_point(points, num_points, point, d);

  vec3 circum_center = GLM_VEC3_ZERO_INIT;
  calc_circumcenter(points, num_points, triangle, circum_center);

  float radius = glm_vec3_distance(circum_center, a);
  float dist = glm_vec3_distance(circum_center, d);
  return radius > dist;
  */
  /*
  if (triangle.tri[0] == 94 && triangle.tri[1] == 34 && triangle.tri[2] == 91) {
    fprintf(stderr, "HIT!\n");
  }
  vec3 a = GLM_VEC3_ZERO_INIT;
  get_point(points, num_points, point, a);

  float dist = glm_vec3_distance(triangle.circum_center, a);
  return triangle.radius > dist;
  */
}

void calc_circumcenter(vec3 *points, size_t num_points, ivec3 triangle,
                       vec3 dest) {
  vec3 a = GLM_VEC3_ZERO_INIT;
  vec3 b = GLM_VEC3_ZERO_INIT;
  vec3 c = GLM_VEC3_ZERO_INIT;
  get_point(points, num_points, triangle[A], a);
  get_point(points, num_points, triangle[B], b);
  get_point(points, num_points, triangle[C], c);

  vec3 AB = GLM_VEC3_ZERO_INIT;
  vec3 AC = GLM_VEC3_ZERO_INIT;
  vec3 BC = GLM_VEC3_ZERO_INIT;
  glm_vec3_sub(b, a, AB);
  glm_vec3_sub(c, a, AC);
  glm_vec3_sub(c, b, BC);

  float D = glm_vec3_angle(AB, AC);

  glm_vec3_negate(AB);
  float E = glm_vec3_angle(AB, BC);

  glm_vec3_negate(AC);
  glm_vec3_negate(BC);
  float F = glm_vec3_angle(AC, BC);

  dest[X] = ((a[X]*sin(2.0*D)) + (b[X]*sin(2.0*E)) + (c[X]*sin(2.0*F))) /
            (sin(2.0*D) + sin(2.0*E) + sin(2.0*F));
  dest[Y] = ((a[Y]*sin(2.0*D)) + (b[Y]*sin(2.0*E)) + (c[Y]*sin(2.0*F))) /
            (sin(2.0*D) + sin(2.0*E) + sin(2.0*F));
  dest[Z] = 0.0;
}

// Get the actual location of a vertex given its index in points. Will account
// for if the desired point is also in the super triangle
void get_point(vec3 *points, size_t num_points, size_t index, vec3 dest) {
  if (index >= num_points) {
    if (index - num_points == A) {
      glm_vec3_copy((vec3) SUPER_TRIANGLE_A, dest);
    } else if (index - num_points == B) {
      glm_vec3_copy((vec3) SUPER_TRIANGLE_B, dest);
    } else {
      glm_vec3_copy((vec3) SUPER_TRIANGLE_C, dest);
    }
    return;
  }

  glm_vec3_copy(points[index], dest);
}

Q_DATA calc_tri_data(vec3 *points, size_t num_points, ivec3 triangle) {
  if (triangle[0] == 89 && triangle[1] == 3 && triangle[2] == 34) {
    fprintf(stderr, "HELLOOOOO\n");
  }
  Q_DATA data;
  glm_ivec3_copy(triangle, data.tri);
  calc_circumcenter(points, num_points, triangle, data.circum_center);

  vec3 a = GLM_VEC3_ZERO_INIT;
  get_point(points, num_points, triangle[A], a);
  data.radius = glm_vec3_distance(data.circum_center, a);

  data.node = INVALID_NODE;
  data.next = INVALID_DATA;
  data.prev = INVALID_DATA;

  return data;
}
