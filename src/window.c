#include <stdio.h>
#include <glad/glad.h>
#include "window.h"

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error (%d): %s\n", error, description);
}

bool window_create(Window *window, int width, int height, const char *title)
{
    if (!window) return false;

    window->width = width;
    window->height = height;
    window->title = title;

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    // hints, we are using 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // i dont think this will ever be run on a mac but ill add it anyway
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window->handle = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window->handle) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window->handle);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD\n");
        glfwDestroyWindow(window->handle);
        glfwTerminate();
        return false;
    }

    // we want vsync
    glfwSwapInterval(1);

    // viewport should be the entire screen
    glViewport(0, 0, width, height);

    return true;
}

void window_update(Window *window)
{
    if (!window || !window->handle) return;

    glfwSwapBuffers(window->handle);
    glfwPollEvents();
}

bool window_should_close(Window *window)
{
    return window && window->handle
        ? glfwWindowShouldClose(window->handle)
        : true;
}

void window_destroy(Window *window)
{
    if (!window) return;

    if (window->handle) {
        glfwDestroyWindow(window->handle);
        window->handle = NULL;
    }

    glfwTerminate();
}
