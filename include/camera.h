#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <stdbool.h>

typedef enum {
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN
} Camera_Movement;

typedef struct {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
} Camera;

void camera_init(Camera* cam, vec3 position, vec3 up, float yaw, float pitch);

void camera_process_keyboard(Camera* cam, Camera_Movement direction, float deltaTime);

void camera_process_mouse(Camera* cam, float xoffset, float yoffset, bool constrainPitch);

void camera_get_view_matrix(Camera* cam, mat4 view);

#endif
