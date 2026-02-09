#pragma once
#include "shader.h"
#include "mesh.h"
#include "camera.h"

bool renderer_init(void);

void renderer_begin(const Camera* camera);

void renderer_draw_mesh(const Mesh* mesh, const Shader* shader, mat4 model);

void renderer_shutdown(void);

