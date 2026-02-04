#include <glad/glad.h>

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "window.h"
#include "renderer.h"
#include "texture.h"


int main(void)
{
    Window window;

    if (!window_create(&window, 800, 600, "GLFW + GLAD")) return 1;

    if (!renderer_init()) return 1;

    // Load texture
    Texture wall;
    if (!texture_load(&wall, "assets/wall.jpg")) {
        fprintf(stderr, "Failed to load texture\n");
        return 1;
    }

    unsigned int shader_program = renderer_get_shader_program();
    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "uTexture"), 0);

    while (!window_should_close(&window)) {
        glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind texture to unit 0
        texture_bind(&wall, 0);

        renderer_draw();

        texture_unbind();
        window_update(&window);
    }

    texture_destroy(&wall);
    renderer_shutdown();
    window_destroy(&window);
    return 0;
}

