#include "ground.h"
#include "renderer.h"
#include "texture.h"
#include <stdio.h>
#include <stdbool.h>
#include <cglm/cglm.h>
#include <glad/glad.h>

bool ground_init(Ground* ground, const char* texture_path, vec3 position, vec2 size, float y, int tiles) {
    if (!ground) return false;

    ground->position[0] = position[0];
    ground->position[1] = position[1];
    ground->position[2] = position[2];
    ground->y = y;
    ground->size[0] = size[0];
    ground->size[1] = size[1];
    ground->tiles = tiles > 0 ? tiles : 1;

    if (!texture_load(&ground->texture, texture_path)) {
        fprintf(stderr, "Failed to load ground texture: %s\n", texture_path);
        return false;
    }

    return true;
}

void ground_draw(Ground* ground) {
    if (!ground) return;

    mat4 model;
    glm_mat4_identity(model);

    glm_translate(model, (vec3){ground->position[0], ground->y, ground->position[2]});

    glm_scale(model, (vec3){ground->size[0], 1.0f, ground->size[1]});

    renderer_set_model(model);

    texture_bind(&ground->texture, 0);

    renderer_draw();
    texture_unbind();
}

void ground_destroy(Ground* ground) {
    if (!ground) return;
    texture_destroy(&ground->texture);
}
