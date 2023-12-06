#include <model.h>

MODEL *init_model(MESH_DATA *md) {
  MODEL *model = malloc(sizeof(MODEL));
  if (model == NULL) {
    fprintf(stderr, "model.c: Failed to allocate model\n");
    exit(1);
  }

  size_t num_verts = md->num_verts * sizeof(VERT);
  size_t num_inds = md->num_inds * sizeof(ivec3);

  glGenVertexArrays(1, &model->VAO);
  glBindVertexArray(model->VAO);

  glGenBuffers(1, &model->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
  glBufferData(GL_ARRAY_BUFFER, num_verts,
               (float *) md->vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                        (void *) 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                        (void *) (3 * sizeof(float)));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                        (void *) (6 * sizeof(float)));
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                        (void *) (9 * sizeof(float)));
  glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float),
                        (void *) (12 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);
  glEnableVertexAttribArray(4);

  glGenBuffers(1, &model->EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_inds, md->indices,
               GL_STATIC_DRAW);

  model->num_indices = num_inds;
  model->texture = INVALID_TEXTURE;

  glBindVertexArray(0);

  return model;
}

MODEL *gen_cube() {
  VERT vertices[] = {
     {{-0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0,1.0,0.0}, {-1.0,0.0,0.0}, {1.0,1.0}},
     {{ 0.5f, -0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0,1.0,0.0}, {-1.0,0.0,0.0}, {0.0,1.0}},
     {{ 0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0,1.0,0.0}, {-1.0,0.0,0.0}, {0.0,0.0}},
     {{-0.5f,  0.5f, -0.5f}, {0.0f,  0.0f, -1.0f}, {0.0,1.0,0.0}, {-1.0,0.0,0.0}, {1.0,0.0}},

     {{-0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0,1.0,0.0}, {1.0,0.0,0.0}, {1.0,1.0}},
     {{ 0.5f, -0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0,1.0,0.0}, {1.0,0.0,0.0}, {0.0,1.0}},
     {{ 0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0,1.0,0.0}, {1.0,0.0,0.0}, {0.0,0.0}},
     {{-0.5f,  0.5f,  0.5f}, {0.0f,  0.0f,  1.0f}, {0.0,1.0,0.0}, {1.0,0.0,0.0}, {1.0,0.0}},

     {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,-1.0,0.0}, {1.0,1.0}},
     {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,-1.0,0.0}, {0.0,1.0}},
     {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,-1.0,0.0}, {0.0,0.0}},
     {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,-1.0,0.0}, {1.0,0.0}},

     {{ 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,1.0,0.0}, {1.0,1.0}},
     {{ 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,1.0,0.0}, {0.0,1.0}},
     {{ 0.5f, -0.5f, -0.5f}, {1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,1.0,0.0}, {0.0,0.0}},
     {{ 0.5f, -0.5f,  0.5f}, {1.0f,  0.0f,  0.0f}, {0.0,0.0,1.0}, {0.0,1.0,0.0}, {1.0,0.0}},

     {{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,-1.0}, {1.0,1.0}},
     {{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,-1.0}, {0.0,1.0}},
     {{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,-1.0}, {0.0,0.0}},
     {{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,-1.0}, {1.0,0.0}},

     {{-0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,1.0}, {1.0,1.0}},
     {{ 0.5f,  0.5f, -0.5f}, {0.0f,  1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,1.0}, {0.0,1.0}},
     {{ 0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,1.0}, {0.0,0.0}},
     {{-0.5f,  0.5f,  0.5f}, {0.0f,  1.0f,  0.0f}, {1.0,0.0,0.0}, {0.0,0.0,1.0}, {1.0,0.0}},
  };

  ivec3 indices[] = {
    {6, 7, 4}, {5, 6, 4},
    {13, 12, 15}, {14, 13, 15},
    {3, 2, 1}, {0, 3, 1},
    {8, 9, 10}, {11, 8, 10},
    {21, 20, 23}, {22, 21, 23},
    {18, 19, 16}, {17, 18, 16}
  };

  MESH_DATA md = {
    vertices,
    indices,
    24, 12
  };

  return init_model(&md);
}

MESH_DATA *gen_mesh(vec3 *points, ivec3 *triangles, size_t num_points,
                    size_t num_tris) {
  VERT *vertices = malloc(sizeof(VERT) * STARTING_BUFF_LEN);
  if (vertices == NULL) {
    fprintf(stderr, "model.c: Unable to allocate vertices.\n");
    exit(1);
  }
  size_t num_verts = 0;
  size_t verts_buf_size = STARTING_BUFF_LEN;

  ivec3 *indices = malloc(sizeof(ivec3) * STARTING_BUFF_LEN);
  if (indices == NULL) {
    fprintf(stderr, "model.c: Unable to allocate indices.\n");
    exit(1);
  }
  size_t num_inds = 0;
  size_t ind_buf_size = STARTING_BUFF_LEN;

  int status = 0;
  for (size_t i = 0; i < num_tris; i++) {
    ivec3 cur_tri = { 0, 0, 0 };
    glm_ivec3_copy(triangles[i], cur_tri);

    for (int i = 0; i < 3; i++) {
      glm_vec3_copy(points[cur_tri[i]], vertices[num_verts].pos);
      glm_vec3_zero(vertices[num_verts].norm);
      glm_vec2_copy(points[cur_tri[i]], vertices[num_verts].tex_pos);

      indices[num_inds][i] = num_verts;

      num_verts++;
      if (num_verts == verts_buf_size) {
        status = double_buffer((void **) &vertices, &verts_buf_size,
                               sizeof(VERT));
        if (status) {
          fprintf(stderr, "model.c: Unable to reallocate vertices\n");
          exit(1);
        }
      }
    }

    num_inds++;
    if (num_inds == ind_buf_size) {
      status = double_buffer((void **) &indices, &ind_buf_size, sizeof(ivec3));
      if (status) {
        fprintf(stderr, "model.c: Unable to reallocate indices\n");
        exit(1);
      }
    }
  }

  MESH_DATA *mesh = malloc(sizeof(MESH_DATA));
  if (mesh == NULL) {
    fprintf(stderr, "model.c: Unable to allocate mesh\n");
    exit(1);
  }
  mesh->vertices = vertices;
  mesh->indices = indices;
  mesh->num_verts = num_verts;
  mesh->num_inds = num_inds;

  return mesh;
}

MESH_DATA *copy_mesh(MESH_DATA *mesh) {
  MESH_DATA *dest = malloc(sizeof(MESH_DATA));
  if (dest == NULL) {
    fprintf(stderr, "model.c: Failed to allocate mesh copy\n");
    exit(1);
  }

  dest->vertices = malloc(sizeof(VERT) * mesh->num_verts);
  if (dest->vertices == NULL) {
    fprintf(stderr, "model.c: Failed to allocate copy of mesh vertices\n");
    exit(1);
  }
  dest->num_verts = mesh->num_verts;

  dest->indices = malloc(sizeof(VERT) * mesh->num_inds);
  if (dest->indices == NULL) {
    fprintf(stderr, "model.c: Failed to allocate copy of mesh indices\n");
    exit(1);
  }
  dest->num_inds = mesh->num_inds;

  for (size_t i = 0; i < dest->num_verts; i++) {
    dest->vertices[i] = mesh->vertices[i];
  }
  for (size_t i = 0; i < dest->num_inds; i++) {
    glm_ivec3_copy(mesh->indices[i], dest->indices[i]);
  }

  return dest;
}

void draw_model(MODEL *model, unsigned int shader) {
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glBindTexture(GL_TEXTURE_2D, model->texture);
  glUniform1i(glGetUniformLocation(shader, "tex"), 0);

  glBindVertexArray(model->VAO);
  glDrawElements(GL_TRIANGLES, model->num_indices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void free_model(MODEL *model) {
  glDeleteBuffers(1, &model->VAO);
  glDeleteBuffers(1, &model->VBO);
  glDeleteBuffers(1, &model->EBO);
  if (model->texture != INVALID_TEXTURE) {
    glDeleteTextures(1, &model->texture);
  }
  free(model);
}

void draw_points(MESH_DATA *md) {
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERT) * md->num_verts, md->vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glDrawArrays(GL_POINTS, 0, md->num_verts);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void free_mesh_data(MESH_DATA *md) {
  free(md->vertices);
  free(md->indices);
  free(md);
}

MESH_DATA read_mesh(char *path) {
  MESH_DATA md = {
    NULL,
    NULL,
    0,
    0
  };

  typedef struct port {
    vec3 position;
    vec3 norm;
    vec2 tex_coords;
  } PORT;

  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    return md;
  }

  if (!fread(&md.num_verts, sizeof(size_t), 1, file)) {
    printf("ERROR\n");
  }
  if (!fread(&md.num_inds, sizeof(size_t), 1, file)) {
    printf("ERROR\n");
  }

  PORT *p = NULL;

  if (md.num_verts) {
    md.vertices = malloc(sizeof(VERT) * md.num_verts);
    p = malloc(sizeof(PORT) * md.num_verts);

    if (md.vertices == NULL) {
      fclose(file);
      md.num_verts = 0;
      md.num_inds = 0;
      fprintf(stderr,
              "model_loader.c: Failed to allocate vertices for model: %s\n",
              path);
      return md;
    }
    if(!fread(p, sizeof(PORT), md.num_verts, file)) {
      printf("ERROR\n");
    }
  }
  for (size_t i = 0; i < md.num_verts; i++) {
    glm_vec3_copy(p[i].position, md.vertices[i].pos); 
    glm_vec3_copy(p[i].norm, md.vertices[i].norm); 
    glm_vec3_copy(p[i].tex_coords, md.vertices[i].tex_pos); 
    glm_vec3_zero(md.vertices[i].tangent);
    glm_vec3_zero(md.vertices[i].bitangent);
  }
  free(p);

  if (md.num_inds) {
    md.indices = malloc(sizeof(int) * 3 * md.num_inds);
    if (md.indices == NULL) {
      fclose(file);
      free(md.vertices);
      md.vertices = NULL;
      md.num_verts = 0;
      md.num_inds = 0;
      fprintf(stderr,
              "model_loader.c: Failed to allocate indices for model: %s\n",
              path);
      return md;
    }
    if (!fread(md.indices, sizeof(int) * 3, md.num_inds, file)) {
      printf("ERROR\n");
    }
  }
  fclose(file);

  for (size_t i = 0; i < md.num_inds; i++) {
    VERT *vert_zero = md.vertices + md.indices[i][0];
    VERT *vert_one = md.vertices + md.indices[i][1];
    VERT *vert_two = md.vertices + md.indices[i][2];
    vec3 e1 = GLM_VEC2_ZERO_INIT;
    vec3 e2 = GLM_VEC2_ZERO_INIT;
    glm_vec3_sub((vert_one->pos), (vert_zero->pos), e1); 
    glm_vec3_sub((vert_two->pos), (vert_zero->pos), e2); 
    float du1 = vert_one->tex_pos[0] - vert_zero->tex_pos[0];
    float du2 = vert_two->tex_pos[0] - vert_zero->tex_pos[0];
    float dv1 = vert_one->tex_pos[1] - vert_zero->tex_pos[1];
    float dv2 = vert_two->tex_pos[1] - vert_zero->tex_pos[1];
    float d_divisor = 1 / ((du1 * dv2) - (du2 * dv1));

    vec3 tang = {
      dv2*e1[X] - dv1*e2[X],
      dv2*e1[Y] - dv1*e2[Y],
      dv2*e1[Z] - dv1*e2[Z]
    };
    glm_vec3_scale(tang, d_divisor, tang);

    vec3 bitang = {
      -du2*e1[X] + du1*e2[X],
      -du2*e1[Y] + du1*e2[Y],
      -du2*e1[Z] + du1*e2[Z]
    };
    glm_vec3_scale(bitang, d_divisor, bitang);

    glm_vec3_normalize(tang);
    glm_vec3_normalize(bitang);
    glm_vec3_copy(tang, vert_zero->tangent);
    glm_vec3_copy(tang, vert_one->tangent);
    glm_vec3_copy(tang, vert_two->tangent);

    glm_vec3_copy(bitang, vert_zero->bitangent);
    glm_vec3_copy(bitang, vert_one->bitangent);
    glm_vec3_copy(bitang, vert_two->bitangent);

    glm_vec3_cross(tang, bitang, vert_zero->norm);
    glm_vec3_cross(tang, bitang, vert_one->norm);
    glm_vec3_cross(tang, bitang, vert_two->norm);

    glm_vec3_cross(bitang, vert_zero->norm, vert_zero->tangent);
    glm_vec3_cross(bitang, vert_one->norm, vert_one->tangent);
    glm_vec3_cross(bitang, vert_two->norm, vert_two->tangent);
  }

  return md;
}

void cheat() {
  s_mesh = (MESH_DATA *) malloc(sizeof(MESH_DATA));
  *s_mesh = read_mesh(DIR "/planet.bin");
  s_model = init_model(s_mesh); 
}

unsigned int gen_texture(char *path) {
  int width;
  int height;
  int num_channels;
  stbi_set_flip_vertically_on_load(1);
  unsigned char *tex_data = stbi_load(path, &width, &height, &num_channels, 0);
  if (tex_data) {
    int format = GL_RGBA;
    if (num_channels == 1) {
      format = GL_RED;
    } else if (num_channels == 2) {
      format = GL_RG;
    } else if (num_channels == 3) {
      format = GL_RGB;
    }

    unsigned int tex = texture_from_buffer(tex_data, width, height, format);
    stbi_image_free(tex_data);

    return tex;
  } else {
    fprintf(stderr, "model_loader.c: Failed to load texture: %s\n", path);
    return INVALID_TEXTURE;
  }
}

unsigned int texture_from_buffer(unsigned char *buffer, int width, int height,
                                 int format) {
  unsigned int tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, buffer);

  return tex;
}
