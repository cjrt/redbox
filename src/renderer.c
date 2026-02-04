#include <glad/glad.h>
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

static const char *vertex_shader_src =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\n";

static const char *fragment_shader_src =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
"}\n";

static unsigned int vao = 0;
static unsigned int vbo = 0;
static unsigned int shader_program = 0;

static char *read_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *data = malloc(size + 1);
    if (!data) {
        fclose(file);
        return NULL;
    }

    fread(data, 1, size, file);
    data[size] = '\0';

    fclose(file);
    return data;
}

static unsigned int compile_shader(unsigned int type, const char *src)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "Shader compile error:\n%s\n", log);
    }

    return shader;
}

bool renderer_init(void)
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    char *vertex_src   = read_file("shaders/vert.shdr");
    char *fragment_src = read_file("shaders/frag.shdr");

    if (!vertex_src || !fragment_src)
        return false;

    unsigned int vertexshader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    unsigned int fragmentshader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);

    free(vertex_src);
    free(fragment_src);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertexshader);
    glAttachShader(shader_program, fragmentshader);
    glLinkProgram(shader_program);

    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,                  // location
        3,                  // size (vec3)
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),  // stride
        (void*)0            // offset
    );
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return true;
}

void renderer_draw(void)
{
    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void renderer_shutdown(void)
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);
}
