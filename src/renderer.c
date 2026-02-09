#include <glad/glad.h>
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

/*

   the renderer should execute draw commands on data it does not own
   if this file is deciding things instead of executing things it is doing too much

   OWNS OPENGL STATE

   input: meshes, shaders, materials, transforms, camera, lights etc
   output: pixels on the screen

*/

static mat4 projection;

bool renderer_init(void) {
   glEnable(GL_DEPTH_TEST);
   return true;
}

void renderer_begin(const Camera* camera) {
   mat4 view;
   camera_get_view_matrix((Camera*)camera, view);

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_draw_mesh(const Mesh* mesh,
                         const Shader* shader,
                         mat4 model)
{
   shader_bind(shader);

   int modelLoc = shader_get_uniform(shader, "model");
   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);

   mesh_draw((Mesh*)mesh);
}

void renderer_shutdown(void) {
   // nothing yet
}
