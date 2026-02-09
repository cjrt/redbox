#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
#include <stdio.h>

/*

   the texture module should only load, store, bind, and unbind textures

   OWNS: GPU texture object

   input: image file or pixel data
   output: GPU texture ID, ready to bind

*/


bool texture_load(Texture *texture, const char *path)
{
    stbi_set_flip_vertically_on_load(0); // Flip vertically: OpenGL origin is bottom-left

    unsigned char *data = stbi_load(path, &texture->width, &texture->height, &texture->channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load texture: %s\n", path);
        return false;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // Set filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set wrapping options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Determine format
    GLenum format = (texture->channels == 4) ? GL_RGBA : GL_RGB;

    // Upload texture to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void texture_bind(const Texture *texture, unsigned int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit); // Activate texture unit
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture_unbind(void)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_destroy(Texture *texture)
{
    glDeleteTextures(1, &texture->id);
    texture->id = 0;
}