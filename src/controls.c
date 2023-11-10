#include <controls.h>

void mouse_pos(GLFWwindow *window, double x, double y) {
}

void mouse_button(GLFWwindow *window, int button, int action, int mods) {
}

void mouse_scroll(GLFWwindow *window, double x_offset, double y_offset) {
}

void keyboard_input(GLFWwindow *window) {
}

void fb_size_callback(GLFWwindow *window, int width, int height) {
  RES_X = width;
  RES_Y = height;
  glViewport(0, 0, RES_X, RES_Y);
}
