#include <glad/glad.h>

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "window.h"
#include "renderer.h"
#include "texture.h"
#include <cglm/cglm.h>


int main(void){
    Window window;
    if(!window_create(&window,800,600,"Rotating Cube")) return 1;

    if(!renderer_init()) return 1;

    // Load texture
    Texture wall;
    if(!texture_load(&wall,"assets/wall.jpg")){
        fprintf(stderr,"Failed to load texture\n");
        return 1;
    }

    // Setup projection & view matrices
    mat4 projection, view;
    glm_perspective(glm_rad(45.0f), 800.0f/600.0f, 0.1f, 100.0f, projection);
    glm_lookat((vec3){3.0f,3.0f,3.0f}, (vec3){0.0f,0.0f,0.0f}, (vec3){0.0f,1.0f,0.0f}, view);

    renderer_set_projection(projection);
    renderer_set_view(view);

    unsigned int shader = renderer_get_shader_program();
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader,"uTexture"),0);

    while(!window_should_close(&window)){
        glClearColor(0.1f,0.1f,0.12f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind texture
        texture_bind(&wall,0);

        // Model matrix — rotating cube
        mat4 model;
        glm_mat4_identity(model);
        glm_rotate(model,(float)glfwGetTime() * glm_rad(50.0f),(vec3){0.5f,1.0f,0.0f});
        renderer_set_model(model);

        renderer_draw();

        texture_unbind();
        window_update(&window);
    }

    texture_destroy(&wall);
    renderer_shutdown();
    window_destroy(&window);
    return 0;
}

