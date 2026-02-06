#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include <stdbool.h>

typedef struct {
    Mesh* meshes;
    int meshCount;
} Model;

bool model_load(Model* model, const char* path);

void model_draw(Model* model);

void model_destroy(Model* model);

#endif
