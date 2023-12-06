#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <const.h>
#include <model_str.h>


#define DIR "./models"
extern MESH_DATA *s_mesh;
extern MODEL *s_model;

// ======================= INTERNALLY DEFINED FUNCTIONS =======================

unsigned int texture_from_buffer(unsigned char *, int, int, int);

// ======================= EXTERNALLY DEFINED FUNCTIONS =======================

int double_buffer(void **, size_t *, size_t);
