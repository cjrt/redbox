#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>

typedef struct {
    unsigned int id;    // OpenGL texture ID
    int width;
    int height;
    int channels;       // Number of color channels (e.g., 3=RGB, 4=RGBA)
} Texture;

/* Load a texture from a file */
bool texture_load(Texture *texture, const char *path);

/* Bind a texture to a texture unit */
void texture_bind(const Texture *texture, unsigned int unit);

/* Unbind texture from the current unit */
void texture_unbind(void);

/* Free the texture from GPU memory */
void texture_destroy(Texture *texture);

#endif