#ifndef GROUND_H
#define GROUND_H

#include "texture.h"
#include <stdbool.h>
#include <cglm/cglm.h>

typedef struct {
    Texture texture;
    vec3 position;   // center position of the ground
    vec2 size;       // width and depth
    float y;
    int tiles;
} Ground;

bool ground_init(Ground* ground, const char* texture_path, vec3 position, vec2 size, float y, int tiles);

void ground_draw(Ground* ground);

void ground_destroy(Ground* ground);

#endif
