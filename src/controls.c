#include <controls.h>

void mouse_pos(GLFWwindow *window, double x, double y) {
  if (!cursor_enabled) {
    if (first_move) {
      first_move = 0;
      prev_x = x;
      prev_y = y;
      return;
    }

    float offset_x = x - prev_x;
    float offset_y = y - prev_y;

    cam_rotate_x(offset_y * MOUSE_SENSITIVITY);
    cam_rotate_y(offset_x * MOUSE_SENSITIVITY);

    prev_x = x;
    prev_y = y;
  }
}

void mouse_button(GLFWwindow *window, int button, int action, int mods) {
  /*
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    DEPTH++;
    printf("depth: %d\n", DEPTH);
    refresh_sphere();
  } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    if (DEPTH > 1.0) {
      DEPTH--;
    }
    printf("depth: %d\n", DEPTH);
    refresh_sphere();
  }
  */
}

void mouse_scroll(GLFWwindow *window, double x_offset, double y_offset) {
  float offset = y_offset * 0.05;
  FREQ += offset;
  printf("freq: %f\n", FREQ);
  refresh_sphere();
}

void keyboard_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS &&
      !holding_grave) {
    if (cursor_enabled) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      cursor_enabled = 0;
    } else {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      cursor_enabled = 1;
    }
    holding_grave = 1;
  } else if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) != GLFW_PRESS) {
    holding_grave = 0;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cam_translate((vec3) {0.0, 0.0, -1.0 * delta_time});
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cam_translate((vec3) {-1.0 * delta_time, 0.0, 0.0});
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cam_translate((vec3) {0.0, 0.0, delta_time});
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cam_translate((vec3) {delta_time, 0.0, 0.0});
  }

  /* Increase Radius */
  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS &&
      !holding_equal) {
    RADIUS += 0.5;
    printf("Radius: %f\n", RADIUS);
    refresh_sphere();
    holding_equal = 1;
  } else if (glfwGetKey(window, GLFW_KEY_EQUAL) != GLFW_PRESS) {
    holding_equal = 0;
  }

  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS &&
      !holding_minus) {
    RADIUS -= 0.5;
    printf("Radius: %f\n", RADIUS);
    refresh_sphere();
    holding_minus = 1;
  } else if (glfwGetKey(window, GLFW_KEY_MINUS) != GLFW_PRESS) {
    holding_minus = 0;
  }
}

void fb_size_callback(GLFWwindow *window, int width, int height) {
  RES_X = width;
  RES_Y = height;
  glViewport(0, 0, RES_X, RES_Y);

  float aspect_ratio = RES_X / RES_Y;
  glm_ortho(-aspect_ratio, aspect_ratio, -1.0, 1.0, 0.0, 100.0, ortho_proj);
  glm_perspective(glm_rad(45.0f), ((float) RES_X) / ((float) RES_Y), 0.1,
                  100.0, persp_proj);
}

void refresh_sphere() {
  MESH_DATA *noisy = copy_mesh(sphere_mesh);
  apply_noise(noisy);
  free_model(sphere);
  sphere = init_model(noisy);
  free_mesh_data(noisy);
}

