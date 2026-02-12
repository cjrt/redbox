#ifndef WINDOW_H

#define WINDOW_H

#include <stdbool.h>
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1920.f
#define WINDOW_HEIGHT 1080.f

typedef struct Window {
    GLFWwindow *handle;
    int width;
    int height;
    const char *title;
} Window;

bool window_create(Window *window, int width, int height, const char *title);

void window_update(Window *window);

bool window_should_close(Window *window);

void window_destroy(Window *window);

#endif