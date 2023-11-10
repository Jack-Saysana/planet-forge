#include <main.h>

int main() {
  GLFWwindow *window = init_gl();

  init_scene();

  while (!glfwWindowShouldClose(window)) {
    keyboard_input(window);

    render_scene(window);
  }

  glfwTerminate();
  return 0;
}
