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
    if(!window_create(&window,1920,1080,"Rotating Cube")) return 1;

    if(!renderer_init()) return 1;

    // Load texture
    Texture wall;
    if(!texture_load(&wall,"assets/wall.jpg")){
        fprintf(stderr,"Failed to load texture\n");
        return 1;
    }

    vec3 cubePositions[10] = {
        {0.0f,0.0f,0.0f}, {2.0f,5.0f,-15.0f}, {-1.5f,-2.2f,-2.5f}, {-3.8f,-2.0f,-12.3f},
        {2.4f,-0.4f,-3.5f}, {-1.7f,3.0f,-7.5f}, {1.3f,-2.0f,-2.5f}, {1.5f,2.0f,-2.5f},
        {1.5f,0.2f,-1.5f}, {-1.1f,-1.4f,1.2f}
    };

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

        for(int i=0;i<10;i++){
            mat4 model;
            glm_mat4_identity(model);
            glm_translate(model, cubePositions[i]);

            float angle = 20.0f * i + (float)glfwGetTime() * 25.0f;
            glm_rotate(model, glm_rad(angle), (vec3){1.0f,0.3f,0.5f});

            renderer_set_model(model);
            renderer_draw();
        }

        texture_unbind();
        window_update(&window);
    }

    texture_destroy(&wall);
    renderer_shutdown();
    window_destroy(&window);
    return 0;
}

