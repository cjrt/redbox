#include "camera.h"
#include <math.h>
#include <cglm/cglm.h>

#define DEFAULT_SPEED 5.0f
#define DEFAULT_SENSITIVITY 0.1f

static void camera_update_vectors(Camera* cam) {
    vec3 front;
    front[0] = cos(glm_rad(cam->Yaw)) * cos(glm_rad(cam->Pitch));
    front[1] = sin(glm_rad(cam->Pitch));
    front[2] = sin(glm_rad(cam->Yaw)) * cos(glm_rad(cam->Pitch));
    glm_normalize_to(front, cam->Front);

    glm_vec3_cross(cam->Front, cam->WorldUp, cam->Right);
    glm_normalize(cam->Right);

    glm_vec3_cross(cam->Right, cam->Front, cam->Up);
    glm_normalize(cam->Up);
}

void camera_init(Camera* cam, vec3 position, vec3 up, float yaw, float pitch) {
    glm_vec3_copy(position, cam->Position);
    glm_vec3_copy(up, cam->WorldUp);
    cam->Yaw = yaw;
    cam->Pitch = pitch;
    cam->MovementSpeed = DEFAULT_SPEED;
    cam->MouseSensitivity = DEFAULT_SENSITIVITY;

    glm_vec3_zero(cam->Front);
    glm_vec3_zero(cam->Right);
    glm_vec3_zero(cam->Up);
    camera_update_vectors(cam);
}

void camera_process_keyboard(Camera* cam, Camera_Movement direction, float deltaTime) {
    float velocity = cam->MovementSpeed * deltaTime;
    vec3 temp;
    switch(direction) {
        case CAMERA_FORWARD:
            glm_vec3_scale(cam->Front, velocity, temp);
            glm_vec3_add(cam->Position, temp, cam->Position);
            break;
        case CAMERA_BACKWARD:
            glm_vec3_scale(cam->Front, velocity, temp);
            glm_vec3_sub(cam->Position, temp, cam->Position);
            break;
        case CAMERA_LEFT:
            glm_vec3_scale(cam->Right, velocity, temp);
            glm_vec3_sub(cam->Position, temp, cam->Position);
            break;
        case CAMERA_RIGHT:
            glm_vec3_scale(cam->Right, velocity, temp);
            glm_vec3_add(cam->Position, temp, cam->Position);
            break;
        case CAMERA_UP:
            glm_vec3_scale(cam->WorldUp, velocity, temp);
            glm_vec3_add(cam->Position, temp, cam->Position);
            break;
        case CAMERA_DOWN:
            glm_vec3_scale(cam->WorldUp, velocity, temp);
            glm_vec3_sub(cam->Position, temp, cam->Position);
            break;
    }
}

void camera_process_mouse(Camera* cam, float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= cam->MouseSensitivity;
    yoffset *= cam->MouseSensitivity;

    cam->Yaw   += xoffset;
    cam->Pitch += yoffset;

    if(constrainPitch) {
        if(cam->Pitch > 89.0f) cam->Pitch = 89.0f;
        if(cam->Pitch < -89.0f) cam->Pitch = -89.0f;
    }

    camera_update_vectors(cam);
}

void camera_get_view_matrix(Camera* cam, mat4 view) {
    vec3 target;
    glm_vec3_add(cam->Position, cam->Front, target);
    glm_lookat(cam->Position, target, cam->Up, view);
}
