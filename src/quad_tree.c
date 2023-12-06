#include <quad_tree.h>

void test_qt() {
  vec4 extents = GLM_VEC4_ZERO_INIT;
  extents[X_MIN] = -16.0;
  extents[X_MAX] =  16.0;
  extents[Y_MIN] = -16.0;
  extents[Y_MAX] =  16.0;

  Q_DATA a = {
    { 0, 0, 0 },
    { 0, 0, 0 },
    16.0,
    INVALID_NODE,
    INVALID_DATA,
    INVALID_DATA
  };

  /*
  Q_DATA b = {
    { 0, 0, 0 },
    { -4.0, -4.0, 0 },
    2.0,
    INVALID_NODE,
    INVALID_DATA,
    INVALID_DATA
  };

  Q_DATA c = {
    { 0, 0, 0 },
    { -1.0, -1.0, 0 },
    1.0,
    INVALID_NODE,
    INVALID_DATA,
    INVALID_DATA
  };

  Q_DATA d = {
    { 0, 0, 0 },
    { 8.0, 4.0, 0 },
    4.0,
    INVALID_NODE,
    INVALID_DATA,
    INVALID_DATA
  };

  Q_DATA e = {
    { 0, 0, 0 },
    { 12.0, -12.0, 0 },
    1.0,
    INVALID_NODE,
    INVALID_DATA,
    INVALID_DATA
  };

  Q_DATA f = {
    { 0, 0, 0 },
    { 0.0, 8.0, 0 },
    2.0,
    INVALID_NODE,
    INVALID_DATA,
    INVALID_DATA
  };
  */

  Q_TREE *tree = init_quad_tree(extents, 5);
  insert_triangle(tree, a);
  remove_triangle(tree, a);
  /*
  insert_triangle(tree, b);
  insert_triangle(tree, c);
  insert_triangle(tree, d);
  insert_triangle(tree, e);
  insert_triangle(tree, f);
  print_tree(tree, 0);

  printf("\n=========== DEL A ===========\n");
  remove_triangle(tree, a);
  print_tree(tree, 0);

  printf("\n=========== DEL F ===========\n");
  remove_triangle(tree, f);
  print_tree(tree, 0);

  Q_DATA *search_res = NULL;
  size_t search_len = search_tree(tree,
                                  (vec2) { -1.0, -1.0 },
                                  &search_res);
  for (size_t i = 0; i < search_len; i++) {
    printf("(%.1f, %.1f): %.1f\n",
           search_res[i].circum_center[X], search_res[i].circum_center[Y],
           search_res[i].radius);
  }
  */
}

void print_tree(Q_TREE *tree, size_t node) {
  Q_NODE *n = tree->nodes + node;
  printf("Node[%ld]:\n", node);
  printf("  data: [");
  if (n->data_head != INVALID_DATA) {
    printf("\n");
    size_t cur = n->data_head;
    do {
      Q_DATA *data = tree->data + cur;
      printf("    (%.1f, %.1f): %f\n",
              data->circum_center[X],
              data->circum_center[Y],
              data->radius);
      cur = data->next;
    } while(cur != n->data_head);
    printf("  ");
  }
  printf("]\n");
  printf("  children: [");
  if (n->first_child != INVALID_NODE) {
    printf(" %ld, %ld, %ld, %ld ]\n",
           n->first_child,
           n->first_child + 1,
           n->first_child + 2,
           n->first_child + 3);

    print_tree(tree, n->first_child);
    print_tree(tree, n->first_child + 1);
    print_tree(tree, n->first_child + 2);
    print_tree(tree, n->first_child + 3);
  } else {
    printf(" N/A ]\n");
  }
}

/*
  Initializes a new quad tree given a max size and depth

  Args:
  - vec4 extents: Describes the minimum and maximum x and y bounds of the
    oct tree
  - size_t max_depth: Describes the zero-indexed max depth of the tree

  Returns:
    Pointer to initialized oct tree
*/
Q_TREE *init_quad_tree(vec4 extents, size_t max_depth) {
  Q_TREE *tree = malloc(sizeof(Q_TREE));
  if (tree == NULL) {
    fprintf(stderr, "quad_tree.c: Unable to allocate quad tree\n");
    return NULL;
  }

  tree->nodes = malloc(sizeof(Q_NODE) * STARTING_BUFF_LEN);
  if (tree->nodes == NULL) {
    fprintf(stderr, "quad_tree.c: Unable to allocate quad tree node buffer\n");
    return NULL;
  }
  tree->node_buf_size = STARTING_BUFF_LEN;

  tree->data = malloc(sizeof(Q_DATA) * STARTING_BUFF_LEN);
  if (tree->data == NULL) {
    fprintf(stderr, "quad_tree.c: Unable to allocate quad tree data buffer\n");
    return NULL;
  }
  tree->data_buf_size = STARTING_BUFF_LEN;

  reset_tree(tree, extents, max_depth);

  return tree;
}

/*
  Resets an oct tree's node and data buffer, as well as updates its extents
  and max depth
*/
void reset_tree(Q_TREE *tree, vec4 extents, size_t max_depth) {
  tree->node_buf_len = 1;
  tree->data_buf_len = 0;
  tree->max_depth = max_depth;

  tree->nodes[0].first_child = INVALID_NODE;
  tree->nodes[0].data_head = INVALID_DATA;
  tree->nodes[0].data_tail = INVALID_DATA;
  glm_vec4_copy(extents, tree->nodes[0].extents);
}

void free_quad_tree(Q_TREE *tree) {
  free(tree->nodes);
  free(tree->data);
  free(tree);
}

/*
  Inserts a triangle into a given oct tree

  Args:
  - Q_TREE *tree: Tree to insert into
  - Q_DATA tri: Triangle to insert
*/
void insert_triangle(Q_TREE *tree, Q_DATA tri) {
  int status = 0;
  size_t ins_index = tree->data_buf_len;
  tree->data[ins_index] = tri;
  tree->data_buf_len++;
  if (tree->data_buf_len == tree->data_buf_size) {
    status = double_buffer((void **) &tree->data, &tree->data_buf_size,
                           sizeof(Q_DATA));
    if (status) {
      fprintf(stderr, "quad_tree.c: Unable to reallocate quad tree data\n");
      exit(1);
    }
  }

  size_t level = 0;
  int quad = 0;
  size_t cur_node = 0;
  while (1) {
    if (level == tree->max_depth) {
      append_to_node(tree, cur_node, ins_index);
      return;
    }

    quad = detect_quad(tree->nodes + cur_node, &tri);
    if (quad == B_LEFT) {
      cur_node = get_child_node(tree, cur_node, 0);
    } else if (quad == B_RIGHT) {
      cur_node = get_child_node(tree, cur_node, 1);
    } else if (quad == T_LEFT) {
      cur_node = get_child_node(tree, cur_node, 2);
    } else if (quad == T_RIGHT) {
      cur_node = get_child_node(tree, cur_node, 3);
    } else {
      append_to_node(tree, cur_node, ins_index);
      return;
    }
    level++;
  }
}

/*
  Removes a triangle from an oct-tree, given a triangle's index

  Args:
  - Q_TREE *tree: Tree to remove from
  - size_t index: Index of triangle to remove
*/
Q_DATA remove_triangle(Q_TREE *tree, Q_DATA tri) {
  Q_DATA *to_delete = NULL;
  size_t to_delete_ind = INVALID_DATA;

  size_t level = 0;
  int quad = 0;
  size_t cur_node = 0;
  while (1) {
    if (level == tree->max_depth) {
      // Search current node
      to_delete_ind = search_node(tree, cur_node, tri.circum_center,
                                  tri.radius);
      if (to_delete_ind != INVALID_TRIANGLE) {
        to_delete = tree->data + to_delete_ind;
      }
      break;
    }

    quad = detect_quad(tree->nodes + cur_node, &tri);
    if (quad == B_LEFT) {
      cur_node = get_child_node(tree, cur_node, 0);
    } else if (quad == B_RIGHT) {
      cur_node = get_child_node(tree, cur_node, 1);
    } else if (quad == T_LEFT) {
      cur_node = get_child_node(tree, cur_node, 2);
    } else if (quad == T_RIGHT) {
      cur_node = get_child_node(tree, cur_node, 3);
    } else {
      // Search current node
      to_delete_ind = search_node(tree, cur_node, tri.circum_center,
                                  tri.radius);
      if (to_delete_ind != INVALID_TRIANGLE) {
        to_delete = tree->data + to_delete_ind;
      }
      break;
    }
    level++;
  }
  if (to_delete == NULL) {
    Q_DATA invalid = {
      { INVALID_POINT, INVALID_POINT, INVALID_POINT },
      { 0.0, 0.0, 0.0 },
      0.0,
      INVALID_NODE,
      INVALID_DATA,
      INVALID_DATA
    };
    return invalid;
  }

  Q_DATA *next = tree->data + to_delete->next;
  Q_DATA *prev = tree->data + to_delete->prev;

  next->prev = to_delete->prev;
  prev->next = to_delete->next;

  Q_NODE *node = tree->nodes + to_delete->node;
  if (node->data_head == to_delete_ind) {
    if (to_delete->next == to_delete_ind) {
      node->data_head = INVALID_DATA;
    } else {
      node->data_head = to_delete->next;
    }
  }
  if (node->data_tail == to_delete_ind) {
    if (to_delete->prev == to_delete_ind) {
      node->data_tail = INVALID_DATA;
    } else {
      node->data_tail = to_delete->prev;
    }
  }

  to_delete->node = INVALID_NODE;
  to_delete->next = INVALID_DATA;
  to_delete->prev = INVALID_DATA;
  Q_DATA ret = *to_delete;

  tree->data_buf_len--;
  if (to_delete_ind != tree->data_buf_len) {
    tree->data[to_delete_ind] = tree->data[tree->data_buf_len];
    if (tree->data[to_delete_ind].next == tree->data_buf_len) {
      tree->data[to_delete_ind].next = to_delete_ind;
    }
    if (tree->data[to_delete_ind].prev == tree->data_buf_len) {
      tree->data[to_delete_ind].prev = to_delete_ind;
    }

    if (tree->data[to_delete_ind].next != INVALID_DATA) {
      next = tree->data + tree->data[to_delete_ind].next;
      next->prev = to_delete_ind;
    }
    if (tree->data[to_delete_ind].prev != INVALID_DATA) {
      prev = tree->data + tree->data[to_delete_ind].prev;
      prev->next = to_delete_ind;
    }

    node = tree->nodes + tree->data[to_delete_ind].node;
    if (node->data_head == tree->data_buf_len) {
      node->data_head = to_delete_ind;
    }
    if (node->data_tail == tree->data_buf_len) {
      node->data_tail = to_delete_ind;
    }
  }

  return ret;
}

/*
  Searches an oct tree for all triangles whose circumcircle could possibly
  contain a given point

  Args:
  - Q_TREE *tree: Tree to search
  - vec2 point: Point of search
  - Q_DATA **result: Pointer to buffer to be populated by search

  RETURNS:
  Length of result buffer
*/
size_t search_tree(Q_TREE *tree, vec2 point, Q_DATA **result) {
  (*result) = malloc(sizeof(Q_DATA) * tree->data_buf_len);
  for (size_t i = 0; i < tree->data_buf_len; i++) {
    (*result)[i] = tree->data[i];
  }
  return tree->data_buf_len;


  (*result) = malloc(sizeof(Q_DATA) * STARTING_BUFF_LEN);
  if (*result == NULL) {
    fprintf(stderr, "quad_tree.c: Failed to allocate resultant buffer\n");
    exit(1);
  }
  size_t res_buf_len = 0;
  size_t res_buf_size = STARTING_BUFF_LEN;

  int level = 0;
  Q_NODE *node = tree->nodes;
  while (1) {
    if (node->data_head != INVALID_DATA) {
      read_buffer(tree, node, result, &res_buf_len, &res_buf_size);
    }
    if (node->first_child == INVALID_NODE || level == tree->max_depth) {
      break;
    }

    int quad = detect_quad_pt(node, point);
    if (quad == B_LEFT) {
      node = tree->nodes + node->first_child;
    } else if (quad == B_RIGHT) {
      node = tree->nodes + node->first_child + 1;
    } else if (quad == T_LEFT) {
      node = tree->nodes + node->first_child + 2;
    } else if (quad == T_RIGHT) {
      node = tree->nodes + node->first_child + 3;
    } else {
      // Read all children
      read_all_children(tree, node, result, &res_buf_len, &res_buf_size);
      break;
    }
    level++;
  }

  return res_buf_len;
}

void read_all_children(Q_TREE *tree, Q_NODE *node, Q_DATA **res,
                       size_t *res_buf_len, size_t *res_buf_size) {
  size_t *node_stack = malloc(sizeof(size_t) * STARTING_BUFF_LEN);
  if (node_stack == NULL) {
    fprintf(stderr, "quad_tree.c: Unable to allocate node stack\n");
    exit(1);
  }
  size_t top = 0;
  size_t stack_size = STARTING_BUFF_LEN;

  int status = -1;
  for (size_t i = 0; i < 4; i++) {
    node_stack[top] = node->first_child + i;
    top++;
    if (top == stack_size) {
      status = double_buffer((void **) &node_stack, &stack_size,
                             sizeof(size_t));
      if (status != 0) {
        fprintf(stderr, "quad_tree.c: Unable to reallocate node stack\n");
        exit(1);
      }
    }
  }

  Q_NODE *cur = NULL;
  while (top) {
    cur = tree->nodes + node_stack[top - 1];
    read_buffer(tree, cur, res, res_buf_len, res_buf_size);
    top--;

    if (cur->first_child != INVALID_NODE) {
      for (size_t i = 0; i < 4; i++) {
        node_stack[top] = cur->first_child + i;
        top++;
        if (top == stack_size) {
          status = double_buffer((void **) &node_stack, &stack_size,
                                 sizeof(size_t));
          if (status != 0) {
            fprintf(stderr, "quad_tree.c: Unable to reallocate node stack\n");
            exit(1);
          }
        }
      }
    }
  }

  free(node_stack);
}

void read_buffer(Q_TREE *tree, Q_NODE *node, Q_DATA **res, size_t *res_buf_len,
                 size_t *res_buf_size) {
  int status = 0;
  Q_DATA *insert_pt = (*res) + *res_buf_len;
  size_t cur = node->data_head;
  do {
    (*insert_pt) = tree->data[cur];
    insert_pt->node = INVALID_NODE;
    insert_pt->next = INVALID_DATA;
    insert_pt->prev = INVALID_DATA;

    (*res_buf_len)++;
    if (*res_buf_len == *res_buf_size) {
      status = double_buffer((void **) res, res_buf_size, sizeof(Q_DATA));
      if (status) {
        fprintf(stderr, "quad_tree.c: Unable to reallocate result buffer\n");
        exit(1);
      }
    }

    insert_pt = (*res) + *res_buf_len;
    cur = tree->data[cur].next;
  } while(cur != node->data_head);
}

void append_to_node(Q_TREE *tree, size_t node_ind, size_t index) {
  Q_NODE *node = tree->nodes + node_ind;
  Q_DATA *data = tree->data + index;
  data->node = node_ind;

  if (node->data_tail == INVALID_DATA) {
    node->data_head = index;
    node->data_tail = index;

    data->next = index;
    data->prev = index;
    return;
  }

  size_t prev = node->data_tail;
  size_t next = node->data_head;

  data->next = next;
  data->prev = prev;

  tree->data[data->prev].next = index;
  tree->data[data->next].prev = index;

  node->data_tail = index;
}

size_t search_node(Q_TREE *tree, size_t node_ind, vec3 circum_center,
                   float radius) {
  Q_NODE *node = tree->nodes + node_ind;
  if (node->data_head == INVALID_DATA) {
    return INVALID_DATA;
  }

  size_t cur_data_ind = node->data_head;
  Q_DATA *cur_data = NULL;
  do {
    cur_data = tree->data + cur_data_ind;
    if (cur_data->circum_center[X] == circum_center[X] &&
        cur_data->circum_center[Y] == circum_center[Y] &&
        cur_data->circum_center[Z] == circum_center[Z] &&
        cur_data->radius == radius) {
      return cur_data_ind;
    }
    cur_data_ind = cur_data->next;
  } while (cur_data_ind != node->data_head);

  return INVALID_DATA;
}

size_t get_child_node(Q_TREE *tree, size_t node_ind, unsigned int child) {
  int status = 0;
  Q_NODE *node = tree->nodes + node_ind;
  Q_NODE *child_node = NULL;
  if (node->first_child == INVALID_NODE) {
    float child_width = 0.5 * (node->extents[X_MAX] - node->extents[X_MIN]);
    node->first_child = tree->node_buf_len;
    for (int i = 0; i < 4; i++) {
      child_node = tree->nodes + tree->node_buf_len;
      child_node->first_child = INVALID_NODE;
      child_node->data_head = INVALID_DATA;
      child_node->data_tail = INVALID_DATA;
      tree->node_buf_len++;

      child_node->extents[X_MIN] = node->extents[X_MIN] +
                                   ((i % 2) * child_width);
      child_node->extents[X_MAX] = child_node->extents[X_MIN] + child_width;
      child_node->extents[Y_MIN] = node->extents[Y_MIN] +
                                   ((i / 2) * child_width);
      child_node->extents[Y_MAX] = child_node->extents[Y_MIN] + child_width;

      if (tree->node_buf_len == tree->node_buf_size) {
        status = double_buffer((void **) &tree->nodes, &tree->node_buf_size,
                               sizeof(Q_NODE));
        if (status) {
          fprintf(stderr,
                  "quad_tree.c: Failed to reallocate tree node buffer\n");
          return INVALID_NODE;
        }
      }
    }
  }

  return node->first_child + child;
}

int detect_quad(Q_NODE *node, Q_DATA *data) {
  int collision = 0;
  vec2 data_pos = GLM_VEC2_ZERO_INIT;
  glm_vec2_copy(data->circum_center, data_pos);

  float quad_width = (node->extents[X_MAX] - node->extents[X_MIN]) / 2.0;
  vec2 t_left = {
    node->extents[X_MIN],
    node->extents[Y_MAX]
  };
  vec2 t_right = {
    node->extents[X_MIN] + quad_width,
    node->extents[Y_MAX]
  };
  vec2 b_left = {
    node->extents[X_MIN],
    node->extents[Y_MAX] - quad_width
  };
  vec2 b_right = {
    node->extents[X_MIN] + quad_width,
    node->extents[Y_MAX] - quad_width
  };

  if(circle_aabb_collision(data_pos, data->radius, t_left, quad_width,
                           quad_width)) {
    collision |= T_LEFT;
  }
  if(circle_aabb_collision(data_pos, data->radius, t_right, quad_width,
                           quad_width)) {
    collision |= T_RIGHT;
  }
  if(circle_aabb_collision(data_pos, data->radius, b_left, quad_width,
                           quad_width)) {
    collision |= B_LEFT;
  }
  if(circle_aabb_collision(data_pos, data->radius, b_right, quad_width,
                           quad_width)) {
    collision |= B_RIGHT;
  }

  return collision;
}

int detect_quad_pt(Q_NODE *node, vec2 point) {
  int collision = 0;

  float quad_width = (node->extents[X_MAX] - node->extents[X_MIN]) / 2.0;
  vec2 t_left = {
    node->extents[X_MIN],
    node->extents[Y_MAX]
  };
  vec2 t_right = {
    node->extents[X_MIN] + quad_width,
    node->extents[Y_MAX]
  };
  vec2 b_left = {
    node->extents[X_MIN],
    node->extents[Y_MAX] - quad_width
  };
  vec2 b_right = {
    node->extents[X_MIN] + quad_width,
    node->extents[Y_MAX] - quad_width
  };

  if(point_aabb_collision(point, t_left, quad_width, quad_width)) {
    collision |= T_LEFT;
  }
  if(point_aabb_collision(point, t_right, quad_width, quad_width)) {
    collision |= T_RIGHT;
  }
  if(point_aabb_collision(point, b_left, quad_width, quad_width)) {
    collision |= B_LEFT;
  }
  if(point_aabb_collision(point, b_right, quad_width, quad_width)) {
    collision |= B_RIGHT;
  }

  return collision;
}

int circle_aabb_collision(vec2 a_center, float a_radius, vec2 b_top_left,
                          float b_width, float b_height) {
  float by_max = b_top_left[Y];
  float by_min = b_top_left[Y] - b_height;
  float bx_max = b_top_left[X] + b_width;
  float bx_min = b_top_left[X];

  vec2 closest_aabb_point = {
    fmax(bx_min, fmin(bx_max, a_center[X])),
    fmax(by_min, fmin(by_max, a_center[Y]))
  };

  float dist_circle_to_closest = glm_vec2_distance(closest_aabb_point,
                                                   a_center);
  float correction_distance = a_radius - dist_circle_to_closest;
  if (correction_distance > 0.0) {
    return 1;
  }

  return 0;
}

int point_aabb_collision(vec2 a_center, vec2 b_top_left, float b_width,
                         float b_height) {
  float by_max = b_top_left[Y];
  float by_min = b_top_left[Y] - b_height;
  float bx_max = b_top_left[X] + b_width;
  float bx_min = b_top_left[X];

  if (a_center[X] < bx_max && a_center[X] > bx_min &&
      a_center[Y] < by_max && a_center[Y] > by_min) {
    return 1;
  } else {
    return 0;
  }
}
