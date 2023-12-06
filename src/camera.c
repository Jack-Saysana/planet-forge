#include <camera.h>

// Node: dir is given in camera space
void cam_translate(vec3 dir) {
  mat4 cam_space = GLM_MAT4_IDENTITY_INIT;
  mat3 to_world_space = GLM_MAT3_IDENTITY_INIT;
  vec3 world_dir = GLM_VEC3_ZERO_INIT;
  if (camera_mode == FLYING) {
    calc_cam_space(cam_space);
    glm_mat4_pick3(cam_space, to_world_space);
    glm_mat3_transpose(to_world_space);

    glm_mat3_mulv(to_world_space, dir, world_dir);
    glm_vec3_add(cam_pos, world_dir, cam_pos);
  } else {
    vec3 cam_x = GLM_VEC3_ZERO_INIT;
    vec3 cam_y = GLM_VEC3_ZERO_INIT;
    vec3 cam_z = GLM_VEC3_ZERO_INIT;

    glm_vec3_sub(cam_pos, planet_pos, cam_y);
    if (cam_y[X] == 0.0 && cam_y[Y] == 0.0 && cam_y[Z] == 0.0) {
      glm_vec3_copy((vec3) {0.0, 1.0, 0.0}, cam_y);
    }
    glm_vec3_normalize(cam_y);
    glm_vec3_copy(cam_right, cam_x);
    glm_vec3_normalize(cam_x);
    glm_vec3_rotate(cam_x, glm_rad(-yaw), cam_y);
    glm_vec3_cross(cam_x, cam_y, cam_z);
    glm_vec3_normalize(cam_z);

    glm_vec3_copy(cam_x, to_world_space[X]);
    glm_vec3_copy(cam_y, to_world_space[Y]);
    glm_vec3_copy(cam_z, to_world_space[Z]);

    glm_mat3_mulv(to_world_space, dir, world_dir);
    glm_vec3_add(cam_pos, world_dir, cam_pos);

    glm_vec3_sub(cam_pos, planet_pos, cam_y);
    if (cam_y[X] == 0.0 && cam_y[Y] == 0.0 && cam_y[Z] == 0.0) {
      glm_vec3_copy((vec3) {0.0, 1.0, 0.0}, cam_y);
    }
    glm_vec3_normalize(cam_y);

    float corr_mag = glm_vec3_dot(cam_y, cam_right);
    vec3 corr_dir = GLM_VEC3_ZERO_INIT;
    glm_vec3_scale_as(cam_y, corr_mag, corr_dir);
    glm_vec3_sub(cam_right, corr_dir, cam_right);
    glm_vec3_normalize(cam_right);
  }
}

void get_cam_loc(vec3 input) {
  glm_vec3_negate_to(cam_pos, input);
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
  vec3 cam_y = GLM_VEC3_ZERO_INIT;
  vec3 cam_z = GLM_VEC3_ZERO_INIT;

  if (camera_mode == FLYING) {
    glm_vec3_copy(base_x, cam_x);
    glm_vec3_copy(base_up, cam_y);
    glm_vec3_negate_to(base_dir, cam_z);
  } else {
    glm_vec3_sub(cam_pos, planet_pos, cam_y);
    if (cam_y[X] == 0.0 && cam_y[Y] == 0.0 && cam_y[Z] == 0.0) {
      glm_vec3_copy((vec3) {0.0, 1.0, 0.0}, cam_y);
    }
    glm_vec3_normalize(cam_y);

    glm_vec3_copy(cam_right, cam_x);
    glm_vec3_normalize(cam_x);
    glm_vec3_cross(cam_x, cam_y, cam_z);
    glm_vec3_normalize(cam_z);
  }

  glm_vec3_rotate(cam_x, glm_rad(-yaw), cam_y);
  glm_vec3_rotate(cam_z, glm_rad(-yaw), cam_y);

  glm_vec3_rotate(cam_y, glm_rad(-pitch), cam_x);
  glm_vec3_rotate(cam_z, glm_rad(-pitch), cam_x);

  mat3 cam_rot = GLM_MAT3_ZERO_INIT;
  glm_vec3_copy(cam_x, cam_rot[X]);
  glm_vec3_copy(cam_y, cam_rot[Y]);
  glm_vec3_copy(cam_z, cam_rot[Z]);
  glm_mat3_transpose(cam_rot);
  glm_mat4_ins3(cam_rot, cam_space);

  vec3 translation = GLM_VEC3_ZERO_INIT;
  glm_vec3_negate_to(cam_pos, translation);
  glm_translate(cam_space, translation);
}

void update_camera() {
  vec3 cam_up = GLM_VEC3_ZERO_INIT;
  glm_vec3_sub(cam_pos, planet_pos, cam_up);
  if (cam_up[X] == 0.0 && cam_up[Y] == 0.0 && cam_up[Z] == 0.0) {
    glm_vec3_copy((vec3) {0.0, 1.0, 0.0}, cam_up);
  }
  glm_vec3_normalize(cam_up);

  if (camera_mode == GROUND) {
    cam_velocity -= delta_time;

    vec2 tex_coords = GLM_VEC2_ZERO_INIT;
    calc_tex_coords(cam_up, tex_coords);

    float displacement = calc_displacement(tex_coords) * RADIUS;
    float dist = glm_vec3_distance(cam_pos, planet_pos);
    if (dist < RADIUS + CAM_HEIGHT + displacement) {
      vec3 correction_dir = GLM_VEC3_ZERO_INIT;
      glm_vec3_scale(cam_up, (RADIUS + CAM_HEIGHT + displacement) - dist,
                     correction_dir);
      glm_vec3_add(cam_pos, correction_dir, cam_pos);
      cam_velocity = 0.0;
    }

    vec3 vel_dir = GLM_VEC3_ZERO_INIT;
    glm_vec3_scale(cam_up, cam_velocity * delta_time, vel_dir);
    glm_vec3_add(cam_pos, vel_dir, cam_pos);
  }
}

void cam_jump() {
  if (camera_mode == GROUND) {
    vec3 cam_up = GLM_VEC3_ZERO_INIT;
    glm_vec3_sub(cam_pos, planet_pos, cam_up);
    if (cam_up[X] == 0.0 && cam_up[Y] == 0.0 && cam_up[Z] == 0.0) {
      glm_vec3_copy((vec3) {0.0, 1.0, 0.0}, cam_up);
    }
    glm_vec3_normalize(cam_up);

    cam_velocity += 2.0;

    vec2 tex_coords = GLM_VEC2_ZERO_INIT;
    calc_tex_coords(cam_up, tex_coords);

    float displacement = calc_displacement(tex_coords) * RADIUS;
    float dist = glm_vec3_distance(cam_pos, planet_pos);
    if (dist < RADIUS + CAM_HEIGHT + displacement) {
      vec3 cam_up = GLM_VEC3_ZERO_INIT;
      glm_vec3_sub(cam_pos, planet_pos, cam_up);
      if (cam_up[X] == 0.0 && cam_up[Y] == 0.0 && cam_up[Z] == 0.0) {
        glm_vec3_copy((vec3) {0.0, 1.0, 0.0}, cam_up);
      }
      glm_vec3_normalize(cam_up);

      vec3 correction_dir = GLM_VEC3_ZERO_INIT;
      glm_vec3_scale(cam_up, (RADIUS + CAM_HEIGHT + displacement) - dist,
                     correction_dir);
      glm_vec3_add(cam_pos, correction_dir, cam_pos);
    }
  }
}

void get_cam_pos(vec3 dest) {
  glm_vec3_copy(cam_pos, dest);
}
