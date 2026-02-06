#include "model.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

bool model_load(Model* model, const char* path) {
    if (!model) return false;

    // Import model using Assimp C API
    const struct aiScene* scene = aiImportFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene) {
        fprintf(stderr, "Assimp error: %s\n", aiGetErrorString());
        return false;
    }

    model->meshCount = scene->mNumMeshes;
    model->meshes = (Mesh*)malloc(sizeof(Mesh) * model->meshCount);

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        struct aiMesh* aimesh = scene->mMeshes[i];
        Mesh* mesh = &model->meshes[i];

        mesh->vertexCount = aimesh->mNumVertices;
        mesh->indexCount = aimesh->mNumFaces * 3;

        // Allocate arrays for vertices (pos+normal+uv) and indices
        float* vertices = (float*)malloc(sizeof(float) * mesh->vertexCount * 8);
        unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * mesh->indexCount);

        for (unsigned int v = 0; v < mesh->vertexCount; v++) {
            vertices[v*8 +0] = aimesh->mVertices[v].x;
            vertices[v*8 +1] = aimesh->mVertices[v].y;
            vertices[v*8 +2] = aimesh->mVertices[v].z;

            if (aimesh->mNormals) {
                vertices[v*8 +3] = aimesh->mNormals[v].x;
                vertices[v*8 +4] = aimesh->mNormals[v].y;
                vertices[v*8 +5] = aimesh->mNormals[v].z;
            } else {
                vertices[v*8 +3] = vertices[v*8 +4] = vertices[v*8 +5] = 0.0f;
            }

            if (aimesh->mTextureCoords[0]) {
                vertices[v*8 +6] = aimesh->mTextureCoords[0][v].x;
                vertices[v*8 +7] = aimesh->mTextureCoords[0][v].y;
            } else {
                vertices[v*8 +6] = vertices[v*8 +7] = 0.0f;
            }
        }

        for (unsigned int f = 0; f < aimesh->mNumFaces; f++) {
            indices[f*3 +0] = aimesh->mFaces[f].mIndices[0];
            indices[f*3 +1] = aimesh->mFaces[f].mIndices[1];
            indices[f*3 +2] = aimesh->mFaces[f].mIndices[2];
        }

        // Upload to OpenGL
        glGenVertexArrays(1, &mesh->VAO);
        glGenBuffers(1, &mesh->VBO);
        glGenBuffers(1, &mesh->EBO);

        glBindVertexArray(mesh->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*mesh->vertexCount*8, vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*mesh->indexCount, indices, GL_STATIC_DRAW);

        // vertex attributes: position (0), normal (1), uv (2)
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        free(vertices);
        free(indices);
    }

    aiReleaseImport(scene);
    return true;
}

void model_draw(Model* model) {
    for (int i = 0; i < model->meshCount; i++) {
        mesh_draw(&model->meshes[i]);
    }
}

void model_destroy(Model* model) {
    for (int i = 0; i < model->meshCount; i++) {
        mesh_destroy(&model->meshes[i]);
    }
    free(model->meshes);
}
