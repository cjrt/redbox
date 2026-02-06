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
#include <cglm/cglm.h>

Camera camera;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    (void)window;
    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera_process_mouse(&camera, xoffset, yoffset, true);
}

void process_input(GLFWwindow* win, float deltaTime, float roomW, float roomH, float roomD){
    if(glfwGetKey(win, GLFW_KEY_W)==GLFW_PRESS) camera_process_keyboard(&camera,CAMERA_FORWARD,deltaTime);
    if(glfwGetKey(win, GLFW_KEY_S)==GLFW_PRESS) camera_process_keyboard(&camera,CAMERA_BACKWARD,deltaTime);
    if(glfwGetKey(win, GLFW_KEY_A)==GLFW_PRESS) camera_process_keyboard(&camera,CAMERA_LEFT,deltaTime);
    if(glfwGetKey(win, GLFW_KEY_D)==GLFW_PRESS) camera_process_keyboard(&camera,CAMERA_RIGHT,deltaTime);
    if(glfwGetKey(win, GLFW_KEY_SPACE)==GLFW_PRESS) camera_process_keyboard(&camera,CAMERA_UP,deltaTime);
    if(glfwGetKey(win, GLFW_KEY_LEFT_SHIFT)==GLFW_PRESS) camera_process_keyboard(&camera,CAMERA_DOWN,deltaTime);

    camera.Position[0] = fmax(-roomW/2 + 0.5f, fmin(camera.Position[0], roomW/2 - 0.5f));
    camera.Position[1] = fmax(0.5f, fmin(camera.Position[1], roomH - 0.5f));
    camera.Position[2] = fmax(-roomD/2 + 0.5f, fmin(camera.Position[2], roomD/2 - 0.5f));
}

int main(void){

    Window window;
    if(!window_create(&window,1920,1080,"Sandbox Room")) return 1;

    if(!renderer_init()) return 1;

    camera_init(&camera,(vec3){0.0f,1.0f,3.0f},(vec3){0.0f,1.0f,0.0f},-90.0f,0.0f);
    glfwSetCursorPosCallback(window.handle,mouse_callback);
    glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Texture floorTex, wallTex, cubeTex;
    if(!texture_load(&floorTex,"assets/grass.png")) { fprintf(stderr,"Failed to load floor\n"); return 1; }
    if(!texture_load(&wallTex,"assets/wall.png")) { fprintf(stderr,"Failed to load wall\n"); return 1; }
    if(!texture_load(&cubeTex,"assets/cube.png")) { fprintf(stderr,"Failed to load cube\n"); return 1; }

    Mesh cubeMesh, planeMesh;
    mesh_init_cube(&cubeMesh);
    mesh_init_plane(&planeMesh, 1.0f, 1.0f, 1); // unit plane, scale in model

    float roomW = 10.0f, roomD = 10.0f, roomH = 5.0f;

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
    glUniform1i(glGetUniformLocation(shader,"uTexture"),0);

    while(!window_should_close(&window)){
        time_update();
        float deltaTime = time_get_delta();

        process_input(window.handle, deltaTime, roomW, roomH, roomD);

        glClearColor(0.53f,0.81f,0.92f,1.0f); // sky color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view;
        camera_get_view_matrix(&camera,view);
        renderer_set_view(view);

        mat4 model;

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
    texture_destroy(&floorTex);
    texture_destroy(&wallTex);
    texture_destroy(&cubeTex);
    renderer_shutdown();
    window_destroy(&window);
    return 0;
}
