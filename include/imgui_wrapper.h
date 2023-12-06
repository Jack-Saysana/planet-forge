int init_imgui(GLFWwindow *);
void render_imgui();
void imgui_cleanup();

int imgui_add_f_slider(const char *, float *, float, float);
int imgui_add_i_slider(const char *, int *, int, int);
