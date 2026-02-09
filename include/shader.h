#pragma once
#include <stdbool.h>

typedef struct {
    unsigned int id;
} Shader;

bool shader_load(Shader* shader,
                 const char* vert_path,
                 const char* frag_path);

void shader_bind(const Shader* shader);
void shader_destroy(Shader* shader);

int shader_get_uniform(const Shader* shader, const char* name);
