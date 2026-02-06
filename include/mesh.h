#ifndef MESH_H
#define MESH_H

#include <stdbool.h>
#include <glad/glad.h>

// A simple mesh structure
typedef struct {
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    int vertexCount;
    int indexCount;
} Mesh;

// Initialize cube mesh
bool mesh_init_cube(Mesh* mesh);

// Initialize plane mesh (XZ plane, centered at origin, tiled)
bool mesh_init_plane(Mesh* mesh, float width, float depth, int tiles);

// Draw a mesh
void mesh_draw(Mesh* mesh);

// Destroy OpenGL buffers
void mesh_destroy(Mesh* mesh);

#endif
