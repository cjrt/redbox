#include "mesh.h"
#include <stdlib.h>
#include <cglm/cglm.h>

// Cube vertices with positions and UVs (no normals)
static float cubeVertices[] = {
    // positions        // texcoords
    -0.5f,-0.5f,-0.5f, 0.0f,0.0f,
     0.5f,-0.5f,-0.5f, 1.0f,0.0f,
     0.5f, 0.5f,-0.5f, 1.0f,1.0f,
    -0.5f, 0.5f,-0.5f, 0.0f,1.0f,

    -0.5f,-0.5f, 0.5f, 0.0f,0.0f,
     0.5f,-0.5f, 0.5f, 1.0f,0.0f,
     0.5f, 0.5f, 0.5f, 1.0f,1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f,1.0f,
};

// Cube indices
static unsigned int cubeIndices[] = {
    0,1,2, 2,3,0, // back
    4,5,6, 6,7,4, // front
    0,4,7, 7,3,0, // left
    1,5,6, 6,2,1, // right
    3,2,6, 6,7,3, // top
    0,1,5, 5,4,0  // bottom
};

bool mesh_init_cube(Mesh* mesh) {
    if(!mesh) return false;

    mesh->vertexCount = 24;
    mesh->indexCount = 36;

    float vertices[] = {
        // positions          // texcoords
        // Back face
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,
         0.5f,-0.5f,-0.5f,   1.0f,0.0f,
         0.5f, 0.5f,-0.5f,   1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,   0.0f,1.0f,

        // Front face
        -0.5f,-0.5f,0.5f,    0.0f,0.0f,
         0.5f,-0.5f,0.5f,    1.0f,0.0f,
         0.5f, 0.5f,0.5f,    1.0f,1.0f,
        -0.5f, 0.5f,0.5f,    0.0f,1.0f,

        // Left face
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,   1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,   1.0f,1.0f,
        -0.5f,-0.5f, 0.5f,   0.0f,1.0f,

        // Right face
         0.5f,-0.5f,-0.5f,   0.0f,0.0f,
         0.5f, 0.5f,-0.5f,   1.0f,0.0f,
         0.5f, 0.5f, 0.5f,   1.0f,1.0f,
         0.5f,-0.5f, 0.5f,   0.0f,1.0f,

        // Top face
        -0.5f,0.5f,-0.5f,    0.0f,0.0f,
         0.5f,0.5f,-0.5f,    1.0f,0.0f,
         0.5f,0.5f,0.5f,     1.0f,1.0f,
        -0.5f,0.5f,0.5f,     0.0f,1.0f,

        // Bottom face
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,
         0.5f,-0.5f,-0.5f,   1.0f,0.0f,
         0.5f,-0.5f,0.5f,    1.0f,1.0f,
        -0.5f,-0.5f,0.5f,    0.0f,1.0f
    };

    unsigned int indices[36];
    for(int i=0;i<6;i++){
        int offset = i*4;
        indices[i*6+0] = offset+0;
        indices[i*6+1] = offset+1;
        indices[i*6+2] = offset+2;
        indices[i*6+3] = offset+2;
        indices[i*6+4] = offset+3;
        indices[i*6+5] = offset+0;
    }

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texcoords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return true;
}

// Plane mesh on XZ plane, centered at origin, simple 2 triangles
bool mesh_init_plane(Mesh* mesh, float width, float depth, int tiles) {
    if(!mesh) return false;
    mesh->vertexCount = 4;
    mesh->indexCount = 6;

    float w2 = width * 0.5f;
    float d2 = depth * 0.5f;

    float uvs = (float)tiles;

    float vertices[] = {
        -w2, 0.0f, -d2, 0.0f, 0.0f,
         w2, 0.0f, -d2, uvs, 0.0f,
         w2, 0.0f,  d2, uvs, uvs,
        -w2, 0.0f,  d2, 0.0f, uvs
    };

    unsigned int indices[] = {0,1,2, 2,3,0};

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texcoords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return true;
}

void mesh_draw(Mesh* mesh) {
    glBindVertexArray(mesh->VAO);
    if(mesh->indexCount > 0)
        glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount);
    glBindVertexArray(0);
}

void mesh_destroy(Mesh* mesh) {
    if(!mesh) return;
    glDeleteBuffers(1, &mesh->VBO);
    if(mesh->indexCount > 0) glDeleteBuffers(1, &mesh->EBO);
    glDeleteVertexArrays(1, &mesh->VAO);
}
