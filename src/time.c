#include "time.h"
#include <GLFW/glfw3.h>

static float deltaTime = 0.0f;
static float lastFrame = 0.0f;

float time_update(void) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    return deltaTime;
}
