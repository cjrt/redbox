#pragma once
#include <GLFW/glfw3.h>
#include "camera.h"

void input_init(GLFWwindow* window);

void input_set_camera(Camera* camera);

void input_update(GLFWwindow* window, float deltaTime, float roomW, float roomH, float roomD);
