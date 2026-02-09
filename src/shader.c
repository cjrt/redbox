#include <glad/glad.h>
#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

/*

   shader should only handle shader compilation, linking, and uniform management
   it should NOT decide rendering order, handle meshes, or manage VAOs/VBOs
   if this file is binding textures or drawing objects, it is doing too much

   OWNS SHADER PROGRAMS AND UNIFORM STATE

   input: shader source files (.vert/.frag), uniform values
   output: compiled shader programs ready for use by the renderer

*/

static char* read_file(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char* data = malloc(size + 1);
    fread(data, 1, size, f);
    data[size] = 0;
    fclose(f);
    return data;
}

static unsigned int compile(unsigned int type, const char* src) {
    unsigned int s = glCreateShader(type);
    glShaderSource(s, 1, &src, NULL);
    glCompileShader(s);
    return s;
}

bool shader_load(Shader* shader,
                 const char* vs_path,
                 const char* fs_path)
{
    char* vs_src = read_file(vs_path);
    char* fs_src = read_file(fs_path);
    if (!vs_src || !fs_src) return false;

    unsigned int vs = compile(GL_VERTEX_SHADER, vs_src);
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fs_src);

    free(vs_src);
    free(fs_src);

    shader->id = glCreateProgram();
    glAttachShader(shader->id, vs);
    glAttachShader(shader->id, fs);
    glLinkProgram(shader->id);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return true;
}

void shader_bind(const Shader* shader) {
    glUseProgram(shader->id);
}

void shader_destroy(Shader* shader) {
    glDeleteProgram(shader->id);
}

int shader_get_uniform(const Shader* shader, const char* name) {
    return glGetUniformLocation(shader->id, name);
}
