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
#include <cglm/cglm.h>

Camera camera;

int main(void){

    Window window;
    if(!window_create(&window,1920,1080,"Sandbox Room")) return 1;

    if(!renderer_init()) return 1;

    camera_init(&camera,(vec3){0.0f,1.0f,3.0f},(vec3){0.0f,1.0f,0.0f},-90.0f,0.0f);
    input_init(window.handle);
	input_set_camera(&camera);

    Texture floorTex, wallTex, cubeTex;
    if(!texture_load(&floorTex,"assets/grass.png")) { fprintf(stderr,"Failed to load floor\n"); return 1; }
    if(!texture_load(&wallTex,"assets/wall.jpg")) { fprintf(stderr,"Failed to load wall\n"); return 1; }
    if(!texture_load(&cubeTex,"assets/cube.png")) { fprintf(stderr,"Failed to load cube\n"); return 1; }

    Mesh cubeMesh, planeMesh;
    mesh_init_cube(&cubeMesh);
    mesh_init_plane(&planeMesh, 1.0f, 1.0f, 1); // unit plane, scale in model

	Model chair;
	if(!model_load(&chair,"assets/source/Chair_Pack/Chair_Pack.obj")){
    	fprintf(stderr,"Failed to load chair model\n");
    	return 1;
	}

	// x,z,y
    float roomW = 30.0f, roomD = 30.0f, roomH = 5.0f;

    vec3 cubePositions[5] = {
        {2.0f,0.5f,-2.0f},
        {-2.0f,0.5f,-1.0f},
        {1.0f,0.5f,2.0f},
        {-1.5f,0.5f,1.5f},
        {0.0f,0.5f,0.0f}
    };

    mat4 projection;
    glm_perspective(glm_rad(45.0f),1920.0f/1080.0f,0.1f,100.0f,projection);
    renderer_set_projection(projection);

    unsigned int shader = renderer_get_shader_program();
    glUseProgram(shader);

	int lightPosLoc = glGetUniformLocation(shader, "lightPos");
	glUniform3f(lightPosLoc, 2.0f, 4.0f, 2.0f);

    glUniform1i(glGetUniformLocation(shader,"uTexture"),0);

    while(!window_should_close(&window)){
        time_update();
        float deltaTime = time_get_delta();

        input_update(window.handle, deltaTime, roomW, roomH, roomD);

        glClearColor(0.53f,0.81f,0.92f,1.0f); // sky color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view;
        camera_get_view_matrix(&camera,view);
        renderer_set_view(view);

        mat4 model;

		mat4 modelMat;
		glm_mat4_identity(modelMat);
		glm_translate(modelMat,(vec3){1.0f,0.0f,-1.0f});
		glm_scale(modelMat,(vec3){0.5f,0.5f,0.5f}); // scale chair
		renderer_set_model(modelMat);
		model_draw(&chair);

        glm_mat4_identity(model);
        glm_scale(model,(vec3){roomW,0.01f,roomD});
        glm_translate(model,(vec3){0.0f,0.0f,0.0f});
        renderer_set_model(model);
        texture_bind(&floorTex,0);
        mesh_draw(&planeMesh);
        texture_unbind();

        glm_mat4_identity(model);
        glm_translate(model,(vec3){0.0f,roomH,0.0f});
        glm_rotate(model,glm_rad(180.0f),(vec3){1.0f,0.0f,0.0f}); // flip down
        glm_scale(model,(vec3){roomW,0.01f,roomD});
        renderer_set_model(model);
        texture_bind(&wallTex,0);
        mesh_draw(&planeMesh);
        texture_unbind();


        // --- Walls ---

        glm_mat4_identity(model);
        glm_translate(model,(vec3){-roomW/2, roomH/2,0.0f});
        glm_rotate(model,glm_rad(90.0f),(vec3){0.0f,0.0f,1.0f});
        glm_scale(model,(vec3){roomH,0.01f,roomD});
        renderer_set_model(model);
        texture_bind(&wallTex,0);
        mesh_draw(&planeMesh);
        texture_unbind();

        glm_mat4_identity(model);
        glm_translate(model,(vec3){roomW/2, roomH/2,0.0f});
        glm_rotate(model,glm_rad(-90.0f),(vec3){0.0f,0.0f,1.0f});
        glm_scale(model,(vec3){roomH,0.01f,roomD});
        renderer_set_model(model);
        texture_bind(&wallTex,0);
        mesh_draw(&planeMesh);
        texture_unbind();

        glm_mat4_identity(model);
        glm_translate(model,(vec3){0.0f, roomH/2, -roomD/2});
        glm_rotate(model,glm_rad(90.0f),(vec3){1.0f,0.0f,0.0f});
        glm_scale(model,(vec3){roomW,0.01f,roomH});
        renderer_set_model(model);
        texture_bind(&wallTex,0);
        mesh_draw(&planeMesh);
        texture_unbind();

        glm_mat4_identity(model);
        glm_translate(model,(vec3){0.0f, roomH/2, roomD/2});
        glm_rotate(model,glm_rad(-90.0f),(vec3){1.0f,0.0f,0.0f});
        glm_scale(model,(vec3){roomW,0.01f,roomH});
        renderer_set_model(model);
        texture_bind(&wallTex,0);
        mesh_draw(&planeMesh);
        texture_unbind();

        texture_bind(&cubeTex,0);
        for(int i=0;i<5;i++){
            glm_mat4_identity(model);
            glm_translate(model,cubePositions[i]);
            float angle = (float)glfwGetTime()*25.0f + i*20.0f;
            glm_rotate(model,glm_rad(angle),(vec3){0.0f,1.0f,0.0f});
            renderer_set_model(model);
            mesh_draw(&cubeMesh);
        }
        texture_unbind();

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
