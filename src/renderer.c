#include <glad/glad.h>
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

static unsigned int shader_program;
static unsigned int VAO, VBO, EBO;

// 8 vertices, position + texcoord
static float vertices[] = {
    // positions        // texcoords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f
};

static unsigned int indices[] = {
    0,1,2, 2,3,0, // back
    4,5,6, 6,7,4, // front
    4,0,3, 3,7,4, // left
    1,5,6, 6,2,1, // right
    4,5,1, 1,0,4, // bottom
    3,2,6, 6,7,3  // top
};

// Uniform locations
static int model_loc, view_loc, proj_loc;

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

bool renderer_init(void) {
    glEnable(GL_DEPTH_TEST); // enable Z-buffer

    // load shaders
    char* vert_src = read_file("shaders/vert.shdr");
    char* frag_src = read_file("shaders/frag.shdr");
    if(!vert_src || !frag_src) return false;

    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vert_src);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, frag_src);
    free(vert_src);
    free(frag_src);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);

    int success;
    char info[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program,512,NULL,info);
        fprintf(stderr,"Program link error: %s\n",info);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    // VAO, VBO, EBO
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // uniform locations
    glUseProgram(shader_program);
    model_loc = glGetUniformLocation(shader_program,"model");
    view_loc  = glGetUniformLocation(shader_program,"view");
    proj_loc  = glGetUniformLocation(shader_program,"projection");

    return true;
}

void renderer_draw(void)
{
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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

void renderer_set_model(mat4 model){
    glUseProgram(shader_program);
    glUniformMatrix4fv(model_loc,1,GL_FALSE,(float*)model);
}
void renderer_set_view(mat4 view){
    glUseProgram(shader_program);
    glUniformMatrix4fv(view_loc,1,GL_FALSE,(float*)view);
}
void renderer_set_projection(mat4 proj){
    glUseProgram(shader_program);
    glUniformMatrix4fv(proj_loc,1,GL_FALSE,(float*)proj);
}