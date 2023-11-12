#include <model.h>

void init_model(MODEL *model, MESH_DATA *md) {
  size_t num_verts = md->num_verts * 8;
  size_t num_inds = md->num_inds * 3;

  glGenVertexArrays(1, &model->VAO);
  glBindVertexArray(model->VAO);

  glGenBuffers(1, &model->VBO);
  glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_verts,
               (float *) md->vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (3 * sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &model->EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * num_inds, md->indices,
               GL_STATIC_DRAW);

  model->num_indices = num_inds;

  glBindVertexArray(0);
}

void draw_model(MODEL *model, unsigned int shader) {
  glUseProgram(shader);
  glActiveTexture(GL_TEXTURE0);
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
