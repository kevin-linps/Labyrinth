#ifndef LABYRINTH_CAMERAXZ_H_
#define LABYRINTH_CAMERAXZ_H_

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

namespace labyrinth
{
    enum CameraMovement { kForward, kBackward, kLeft, kRight };

    class CameraXZ
    {
    private:
        glm::vec3 world_up_{ 0.0f, 1.0f, 0.0f };
        float Yaw{ 0.0f };
        float Pitch{ 0.0f };
        float y_offset;

        float MovementSpeed{ 20.0f };
        float MouseSensitivity{ 0.1f };

        void UpdateCameraVectors();

    public:
        glm::vec3 view_position_;
        glm::vec3 view_front_;
        glm::vec3 view_up_;
        glm::vec3 view_right_;

        CameraXZ(glm::vec3 position);

        glm::mat4 GetViewMatrix();

        void ProcessKeyboard(CameraMovement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset);
    };
}  // namespace labyrinth
#endif