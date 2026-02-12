#include <glad/glad.h>
#include "renderer.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include <stdio.h>
#include <stdlib.h>

/*

   the renderer should execute draw commands on data it does not own
   if this file is deciding things instead of executing things it is doing too much

   OWNS OPENGL STATE

   input: meshes, shaders, materials, transforms, camera, lights etc
   output: pixels on the screen

*/

static Shader* activeShader = NULL;
static mat4 projection;
static mat4 view;
static vec3 lightPos;

bool renderer_init(void) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return true;
}

void renderer_shutdown(void) {
    // todo
}

void renderer_clear(vec4 color) {
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_set_shader(Shader* shader) {
    activeShader = shader;
    shader_bind(shader);
}

void renderer_set_projection(mat4 proj) {
    if (!activeShader) return;
    shader_bind(activeShader);
    int loc = shader_get_uniform(activeShader, "projection");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)proj);
    glm_mat4_copy(proj, projection);
}

void renderer_set_view(mat4 v) {
    if (!activeShader) return;
    shader_bind(activeShader);
    int loc = shader_get_uniform(activeShader, "view");
    glUniformMatrix4fv(loc, 1, GL_FALSE, (float*)v);
    glm_mat4_copy(v, view);
}

void renderer_set_light(vec3 pos) {
    lightPos[0] = pos[0];
    lightPos[1] = pos[1];
    lightPos[2] = pos[2];
    if (!activeShader) return;
    shader_bind(activeShader);
    int loc = shader_get_uniform(activeShader, "lightPos");
    glUniform3fv(loc, 1, lightPos);
}

void renderer_draw_mesh(const Mesh* mesh, const Texture* tex, mat4 model) {
    if (!activeShader) return;

    shader_bind(activeShader);

    // set uniforms
    int modelLoc = shader_get_uniform(activeShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);

    // bind texture
    if (tex) texture_bind((Texture*)tex, 0);

    // draw
    mesh_draw((Mesh*)mesh);

    // unbind texture
    if (tex) texture_unbind();
}

void renderer_draw_model(const Model* model, mat4 modelMatrix) {
    if (!activeShader) return;

    shader_bind(activeShader);

    int modelLoc = shader_get_uniform(activeShader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)modelMatrix);

    model_draw((Model*)model);
}

void renderer_draw_wall(const Mesh* plane, const Texture* tex,
                        float roomW, float roomD, float roomH) {
    mat4 model;

    // left wall
    glm_mat4_identity(model);
    glm_translate(model, (vec3){-roomW/2, roomH/2, 0.0f});
    glm_rotate(model, glm_rad(90.0f), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(model, (vec3){roomH, 0.01f, roomD});
    renderer_draw_mesh(plane, tex, model);

    // right wall
    glm_mat4_identity(model);
    glm_translate(model, (vec3){roomW/2, roomH/2, 0.0f});
    glm_rotate(model, glm_rad(-90.0f), (vec3){0.0f, 0.0f, 1.0f});
    glm_scale(model, (vec3){roomH, 0.01f, roomD});
    renderer_draw_mesh(plane, tex, model);

    // back wall
    glm_mat4_identity(model);
    glm_translate(model, (vec3){0.0f, roomH/2, -roomD/2});
    glm_rotate(model, glm_rad(90.0f), (vec3){1.0f, 0.0f, 0.0f});
    glm_scale(model, (vec3){roomW, 0.01f, roomH});
    renderer_draw_mesh(plane, tex, model);

    // front wall
    glm_mat4_identity(model);
    glm_translate(model, (vec3){0.0f, roomH/2, roomD/2});
    glm_rotate(model, glm_rad(-90.0f), (vec3){1.0f, 0.0f, 0.0f});
    glm_scale(model, (vec3){roomW, 0.01f, roomH});
    renderer_draw_mesh(plane, tex, model);
}
