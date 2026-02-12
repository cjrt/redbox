#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>
#include "window.h"
#include "renderer.h"
#include "texture.h"
#include "camera.h"
#include "time.h"
#include "mesh.h"
#include "model.h"
#include "input.h"
#include "shader.h"
#include <cglm/cglm.h>

/*

   main.c should only orchestrate the engine:
   - create window
   - initialize renderer, camera, textures, meshes, models
   - update input
   - update game state
   - call renderer to draw the scene

   it should NOT directly create VAOs, bind buffers, or manage shader compilation
   it should NOT contain hardcoded geometry transformations inline

   OWNS: high-level scene and engine orchestration

   input: engine modules (renderer, camera, mesh, texture, model)
   output: runs game loop, sends draw commands to renderer

*/

Camera camera;

int main(void) {

    Window window;
    if (!window_create(&window, WINDOW_WIDTH, WINDOW_HEIGHT, "redbox")) return 1;

    if (!renderer_init()) return 1;

    camera_init(&camera, (vec3){0.0f,1.0f,3.0f}, (vec3){0.0f,1.0f,0.0f}, -90.0f, 0.0f);
    input_init(window.handle);
    input_set_camera(&camera);

    Texture floorTex, wallTex, cubeTex;
    if (!texture_load(&floorTex, "assets/grass.png") ||
        !texture_load(&wallTex, "assets/wall.jpg") ||
        !texture_load(&cubeTex, "assets/cube.png")) {
        fprintf(stderr, "Failed to load textures\n");
        return 1;
    }

    Mesh cubeMesh, planeMesh;
    mesh_init_cube(&cubeMesh);
    mesh_init_plane(&planeMesh, 1.0f, 1.0f, 1); // unit plane, scaling in model matrix

    Model chair;
    if (!model_load(&chair, "assets/source/Chair_Pack/Chair_Pack.obj")) {
        fprintf(stderr, "Failed to load chair model\n");
        return 1;
    }

    float roomW = 100.0f, roomD = 100.0f, roomH = 20.0f;
    vec3 cubePositions[5] = {
        {2.0f,0.5f,-2.0f},
        {-2.0f,0.5f,-1.0f},
        {1.0f,0.5f,2.0f},
        {-1.5f,0.5f,1.5f},
        {0.0f,0.5f,0.0f}
    };

    Shader basicShader;
    shader_load(&basicShader, "shaders/vert.shdr", "shaders/frag.shdr");
    renderer_set_shader(&basicShader);

    mat4 projection;
    glm_perspective(glm_rad(45.0f), WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f, projection);
    renderer_set_projection(projection);

    renderer_set_light((vec3){2.0f,4.0f,2.0f});

    while (!window_should_close(&window)) {
        float deltaTime = time_update();

        input_update(window.handle, deltaTime, roomW, roomH, roomD);

        renderer_clear((vec4){0.53f, 0.81f, 0.92f, 1.0f}); // sky color

        // camera view
        mat4 view;
        camera_get_view_matrix(&camera, view);
        renderer_set_view(view);

        // draw floor
        mat4 floorModel;
        glm_mat4_identity(floorModel);
        glm_scale(floorModel, (vec3){roomW,0.01f,roomD});
        renderer_draw_mesh(&planeMesh, &floorTex, floorModel);

        // draw ceiling
        mat4 ceilingModel;
        glm_mat4_identity(ceilingModel);
        glm_translate(ceilingModel, (vec3){0.0f, roomH, 0.0f});
        glm_rotate(ceilingModel, glm_rad(180.0f), (vec3){1.0f,0.0f,0.0f});
        glm_scale(ceilingModel, (vec3){roomW,0.01f,roomD});
        renderer_draw_mesh(&planeMesh, &wallTex, ceilingModel);

        // draw walls
        renderer_draw_wall(&planeMesh, &wallTex, roomW, roomD, roomH);

        // draw cubes
        for (int i=0; i<5; i++) {
            mat4 cubeModel;
            glm_mat4_identity(cubeModel);
            glm_translate(cubeModel, cubePositions[i]);
            float angle = (float)glfwGetTime()*25.0f + i*20.0f;
            glm_rotate(cubeModel, glm_rad(angle), (vec3){0.0f,1.0f,0.0f});
            renderer_draw_mesh(&cubeMesh, &cubeTex, cubeModel);
        }

        // draw model
        mat4 chairModel;
        glm_mat4_identity(chairModel);
        glm_translate(chairModel, (vec3){1.0f,0.0f,-1.0f});
        glm_scale(chairModel, (vec3){0.5f,0.5f,0.5f});
        renderer_draw_model(&chair, chairModel);

        window_update(&window);
    }

    mesh_destroy(&cubeMesh);
    mesh_destroy(&planeMesh);
    model_destroy(&chair);
    texture_destroy(&floorTex);
    texture_destroy(&wallTex);
    texture_destroy(&cubeTex);
    renderer_shutdown();
    window_destroy(&window);

    return 0;
}

