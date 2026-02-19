#pragma once
#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "texture.h"
#include <cglm/cglm.h>

bool renderer_init(void);

void renderer_shutdown(void);

void renderer_set_shader(Shader *shader);

void renderer_set_projection(mat4 proj);

void renderer_set_view(mat4 view);

void renderer_set_light(vec3 pos);

void renderer_clear(vec4 color);

void renderer_draw_mesh(const Mesh *mesh, const Texture *tex, mat4 model);

void renderer_draw_model(const Model *model, mat4 modelMatrix);

void renderer_draw_room(const Mesh *plane, const Texture *tex, float roomW,
                        float roomD, float roomH);
