#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <cglm/cglm.h> // for mat4

bool renderer_init(void);

void renderer_draw(void);

// free resources
void renderer_shutdown(void);

unsigned int renderer_get_shader_program(void);

void renderer_set_model(mat4 model);

void renderer_set_view(mat4 view);

void renderer_set_projection(mat4 proj);

#endif
