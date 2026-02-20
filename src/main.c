// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// leave this alone clang
#include "camera.h"
#include "input.h"
#include "mesh.h"
#include "model.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "time.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <stdbool.h>
#include <stdio.h>

/*

   main.c should only orchestrate the engine:
   - create window
   - initialize renderer, camera, textures, meshes, models
   - update input
   - update game state
   - call renderer to draw the scene

   it should NOT directly create VAOs, bind buffers, or manage shader
   compilation it should NOT contain hardcoded geometry transformations inline

   OWNS: high-level scene and engine orchestration

   input: engine modules (renderer, camera, mesh, texture, model)
   output: runs game loop, sends draw commands to renderer

*/

Camera camera;

int main(void) {

  Window window;
  if (!window_create(&window, WINDOW_WIDTH, WINDOW_HEIGHT, "redbox"))
    return 1;

  if (!renderer_init())
    return 1;

  camera_init(&camera, (vec3){0.0f, 1.0f, 3.0f}, (vec3){0.0f, 1.0f, 0.0f},
              -90.0f, 0.0f);
  input_init(window.handle);
  input_set_camera(&camera);

  Texture floorTex, wallTex, cubeTex;
  if (!texture_load(&floorTex, "assets/grass.png") ||
      !texture_load(&wallTex, "assets/wall.jpg") ||
      !texture_load(&cubeTex, "assets/cube.png")) {
    fprintf(stderr, "Failed to load textures\n");
    return 1;
  }

  Mesh planeMesh;
  mesh_init_plane(&planeMesh, 1.0f, 1.0f,
                  1); // unit plane, scaling in model matrix

  Model chair;
  if (!model_load(&chair, "assets/source/Chair_Pack/Chair_Pack.obj")) {
    fprintf(stderr, "Failed to load chair model\n");
    return 1;
  }

  float roomW = 100.0f, roomD = 100.0f, roomH = 20.0f;

  Shader basicShader;
  shader_load(&basicShader, "shaders/vert.shdr", "shaders/frag.shdr");
  renderer_set_shader(&basicShader);

  mat4 projection;
  glm_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 100.0f,
                  projection);
  renderer_set_projection(projection);

  renderer_set_light((vec3){2.0f, 4.0f, 2.0f});

  while (!window_should_close(&window)) {
    float deltaTime = time_update();

    input_update(window.handle, deltaTime, roomW, roomH, roomD);

    renderer_clear((vec4){0.53f, 0.81f, 0.92f, 1.0f}); // sky color

    // camera view
    mat4 view;
    camera_get_view_matrix(&camera, view);
    renderer_set_view(view);

    // floor 
    renderer_draw_quad(&planeMesh, &floorTex, (vec3){0, 0, 0}, roomW, roomD, PLANE_FLOOR);

    // front wall
    renderer_draw_quad(&planeMesh, &wallTex, (vec3){0.0f, 1.5f, -5.0f}, 5.0f, 3.0f, PLANE_WALL_Z);

    // side wall
    renderer_draw_quad(&planeMesh, &wallTex, (vec3){3.0f, 1.5f, 0.0f}, 4.0f, 3.0f, PLANE_WALL_X);

    // draw model
    mat4 chairModel;
    glm_mat4_identity(chairModel);
    glm_translate(chairModel, (vec3){1.0f, 0.0f, -1.0f});
    glm_scale(chairModel, (vec3){0.5f, 0.5f, 0.5f});
    renderer_draw_model(&chair, chairModel);

    window_update(&window);
  }

  mesh_destroy(&planeMesh);
  model_destroy(&chair);
  texture_destroy(&cubeTex);
  renderer_shutdown();
  window_destroy(&window);

  return 0;
}
