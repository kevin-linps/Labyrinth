#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace labyrinth;

CameraXZ::CameraXZ(glm::vec3 position)
{
    view_position_ = position;
    y_offset = position.y;

    UpdateCameraVectors();
}

glm::mat4 CameraXZ::GetViewMatrix()
{
    return glm::lookAt(view_position_, view_position_ + view_front_, view_up_);
}

void CameraXZ::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == kForward) 
        view_position_ += view_front_ * velocity;
    if (direction == kBackward) 
        view_position_ -= view_front_ * velocity;
    if (direction == kLeft) 
        view_position_ -= view_right_ * velocity;
    if (direction == kRight) 
        view_position_ += view_right_ * velocity;

    view_position_.y = y_offset;
}

void CameraXZ::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f) Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    UpdateCameraVectors();
}

void CameraXZ::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    view_front_ = glm::normalize(front);
    view_right_ = glm::normalize(glm::cross(view_front_, world_up_));
    view_up_ = glm::normalize(glm::cross(view_right_, view_front_));
}
