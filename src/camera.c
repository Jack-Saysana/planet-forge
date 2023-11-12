#include <camera.h>

// Node: dir is given in camera space
void cam_translate(vec3 dir) {
  mat4 cam_space = GLM_MAT4_IDENTITY_INIT;
  calc_cam_space(cam_space);
  mat3 to_world_space = GLM_MAT3_IDENTITY_INIT;
  glm_mat4_pick3(cam_space, to_world_space);
  glm_mat3_transpose(to_world_space);

  vec3 world_dir = GLM_VEC3_ZERO_INIT;
  glm_mat3_mulv(to_world_space, dir, world_dir);

  glm_vec3_sub(cam_pos, world_dir, cam_pos);
}

void cam_rotate_x(float angle) {
  pitch += angle;
  if (pitch > 89.0) {
    pitch = 89.0;
  } else if (pitch < -89.0) {
    pitch = -89.0;
  }
}

void cam_rotate_y(float angle) {
  yaw += angle;
}

void calc_cam_space(mat4 cam_space) {
  glm_mat4_identity(cam_space);
  vec3 cam_x = GLM_VEC3_ZERO_INIT;
  glm_vec3_copy(base_x, cam_x);
  vec3 cam_y = GLM_VEC3_ZERO_INIT;
  glm_vec3_copy(base_up, cam_y);
  vec3 cam_z = GLM_VEC3_ZERO_INIT;
  glm_vec3_negate_to(base_dir, cam_z);

  glm_vec3_rotate(cam_y, glm_rad(pitch), cam_x);
  glm_vec3_rotate(cam_z, glm_rad(pitch), cam_x);

  glm_vec3_rotate(cam_x, glm_rad(yaw), cam_y);
  glm_vec3_rotate(cam_z, glm_rad(yaw), cam_y);

  glm_vec3_copy(cam_x, cam_space[X]);
  glm_vec3_copy(cam_y, cam_space[Y]);
  glm_vec3_copy(cam_z, cam_space[Z]);

  glm_translate(cam_space, cam_pos);
}
