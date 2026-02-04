#include <glad/glad.h>
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned int shader_program;
static unsigned int VAO, VBO;

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file) { fprintf(stderr, "Failed to open %s\n", path); return NULL; }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);
    char *data = malloc(size + 1);
    fread(data, 1, size, file);
    data[size] = '\0';
    fclose(file);
    return data;
}

static unsigned int compile_shader(unsigned int type, const char *source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader compilation error: %s\n", infoLog);
    }
    return shader;
}

bool renderer_init(void)
{
    // Load shaders
    char *vert_src = read_file("shaders/vert.shdr");
    char *frag_src = read_file("shaders/frag.shdr");
    if (!vert_src || !frag_src) return false;

    unsigned int vertexshader = compile_shader(GL_VERTEX_SHADER, vert_src);
    unsigned int fragmentshader = compile_shader(GL_FRAGMENT_SHADER, frag_src);

    free(vert_src);
    free(frag_src);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexshader);
    glAttachShader(shader_program, fragmentshader);
    glLinkProgram(shader_program);

    // Check link status
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        fprintf(stderr, "Shader program link error: %s\n", infoLog);
    }

    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    float vertices[] = {
        // positions        // tex
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.5f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // tex
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void renderer_draw(void)
{
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void renderer_shutdown(void)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);
}

unsigned int renderer_get_shader_program(void)
{
    return shader_program;
}