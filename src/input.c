// aa
#include <glad/glad.h>
// aa
#include "input.h"
#include <math.h>
#include <stdbool.h>

#include "camera.h"

static Camera *camera = NULL;

static float lastX = 800.0f / 2.0f;
static float lastY = 600.0f / 2.0f;
static bool firstMouse = true;

static bool wireframe = false;
static bool wireframeKeyPressed = false;

static float groundHeight = 0.0f; // y-coordinate of the floor
static float playerHeight = 1.8f; // camera height above the ground

static void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  (void)window;

  if (!camera)
    return;

  if (firstMouse) {
    lastX = (float)xpos;
    lastY = (float)ypos;
    firstMouse = false;
  }

  float xoffset = (float)xpos - lastX;
  float yoffset = lastY - (float)ypos;

  lastX = (float)xpos;
  lastY = (float)ypos;

  camera_process_mouse(camera, xoffset, yoffset, true);
}

// --------------------------------------------------
// Public API
// --------------------------------------------------

void input_set_camera(Camera *cam) { camera = cam; }

void input_init(GLFWwindow *window) {
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void input_update(GLFWwindow *win, float deltaTime, float roomW, float roomH,
                  float roomD) {
  if (!camera)
    return;

  // Movement
  if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
    camera_process_keyboard(camera, CAMERA_FORWARD, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
    camera_process_keyboard(camera, CAMERA_BACKWARD, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
    camera_process_keyboard(camera, CAMERA_LEFT, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
    camera_process_keyboard(camera, CAMERA_RIGHT, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera_process_keyboard(camera, CAMERA_UP, deltaTime);
  if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera_process_keyboard(camera, CAMERA_DOWN, deltaTime);

  // Wireframe toggle (F1)
  if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS && !wireframeKeyPressed) {
    wireframe = !wireframe;
    wireframeKeyPressed = true;

    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
  }

  if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_RELEASE) {
    wireframeKeyPressed = false;
  }

  // Room bounds + fixed player height
  camera->Position[0] = fmaxf(-roomW / 2.0f + 0.5f,
                              fminf(camera->Position[0], roomW / 2.0f - 0.5f));

  camera->Position[1] = groundHeight + playerHeight - 1.0f;

  camera->Position[2] = fmaxf(-roomD / 2.0f + 0.5f,
                              fminf(camera->Position[2], roomD / 2.0f - 0.5f));
}
