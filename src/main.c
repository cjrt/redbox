#include <glad/glad.h>

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "window.h"
#include "renderer.h"


int main(void)
{
    Window window;

    if (!window_create(&window, 800, 600, "GLFW + GLAD")) {
        return 1;
    }

    renderer_init();

    while (!window_should_close(&window)) {
        glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer_draw();

        window_update(&window);
    }

    renderer_shutdown();
    window_destroy(&window);
    return 0;
}
