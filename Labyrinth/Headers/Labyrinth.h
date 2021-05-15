#ifndef LABYRINTH_LABYRINTH_H_
#define LABYRINTH_LABYRINTH_H_

#include "Camera.h"
#include "Engine.h"
#include "Maze.h"

namespace labyrinth
{
    class Labyrinth : public moe::Engine
    {
    private:
        int maze_row_{ 8 };
        int maze_col_{ 8 };
        Maze maze_{ 8, 8 };

        float cell_width_ = 15.00f;
        float cell_height_ = 10.0f;
        float wall_thickness_ = 1.0f;

        CameraXZ camera{ glm::vec3(0.5, 5, 0.5) };

        double last_frame_;
        double last_mouse_x_ = 0.0f;
        double last_mouse_y_ = 0.0f;
        glm::vec3 position_;

        virtual void RendorScene();
        virtual void ProcessKeyEvents();
        virtual void ProcessMouseEvents();

    public:
        using Engine::Engine;

        void Initialize();
    };
}  // namespace labyrinth
#endif