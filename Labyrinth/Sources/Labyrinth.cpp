#include "Labyrinth.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

using namespace labyrinth;

void Labyrinth::Initialize()
{
    {
        camera.view_position_.x = cell_width_ / 2.0f;
        camera.view_position_.z = cell_width_ / 2.0f;
        position_ = camera.view_position_;

        last_frame_ = glfwGetTime();
        glfwGetCursorPos(window_, &last_mouse_x_, &last_mouse_y_);
    }

    {  // Generate walls of the maze
        std::vector<GLfloat> wall_vertices;
        std::vector<GLuint> wall_indices;
        int num_coords = 0;

        for (int x = 0; x < maze_col_; x++)
        {
            for (int y = 0; y < maze_row_; y++)
            {
                int index = x + y * maze_col_;

                GLfloat x1 = x * cell_width_ + wall_thickness_;
                GLfloat x2 = (x + 1) * cell_width_ - wall_thickness_;
                GLfloat y1 = 0.00f;
                GLfloat y2 = cell_height_;
                GLfloat z1 = y * cell_width_ + wall_thickness_;
                GLfloat z2 = (y + 1) * cell_width_ - wall_thickness_;

                if (!(maze_.structure_.at(index) & kConnectsLeft))
                {
                    GLfloat vertices[] = {
                        x1,   y2,   z2,   1.0f, 0.0f, 0.0f, 0.0f, 1.0f, x1,   y2,   z1,
                        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, x1,   y1,   z1,   1.0f, 0.0f, 0.0f,
                        1.0f, 0.0f, x1,   y1,   z2,   1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                    };
                    GLuint indices[] = {
                        0u + num_coords, 1u + num_coords, 2u + num_coords,
                        0u + num_coords, 2u + num_coords, 3u + num_coords,
                    };

                    wall_vertices.insert(wall_vertices.end(), vertices, vertices + 32);
                    wall_indices.insert(wall_indices.end(), indices, indices + 6);
                    num_coords += 4;
                }
                if (!(maze_.structure_.at(index) & kConnectsRight))
                {
                    GLfloat vertices[] = {
                        x2, y2, z2, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                        x2, y2, z1, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                        x2, y1, z1, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                        x2, y1, z2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                    };
                    GLuint indices[] = {
                        0u + num_coords, 1u + num_coords, 2u + num_coords,
                        0u + num_coords, 2u + num_coords, 3u + num_coords,
                    };

                    wall_vertices.insert(wall_vertices.end(), vertices, vertices + 32);
                    wall_indices.insert(wall_indices.end(), indices, indices + 6);
                    num_coords += 4;
                }
                if (!(maze_.structure_.at(index) & kConnectsUp))
                {
                    GLfloat vertices[] = {
                        x1,   y2,   z1,   0.0f, 0.0f, 1.0f, 0.0f, 1.0f, x2,   y2,   z1,
                        0.0f, 0.0f, 1.0f, 1.0f, 1.0f, x2,   y1,   z1,   0.0f, 0.0f, 1.0f,
                        1.0f, 0.0f, x1,   y1,   z1,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                    };
                    GLuint indices[] = {
                        0u + num_coords, 1u + num_coords, 2u + num_coords,
                        0u + num_coords, 2u + num_coords, 3u + num_coords,
                    };

                    wall_vertices.insert(wall_vertices.end(), vertices, vertices + 32);
                    wall_indices.insert(wall_indices.end(), indices, indices + 6);
                    num_coords += 4;
                }
                if (!(maze_.structure_.at(index) & kConnectsDown))
                {
                    GLfloat vertices[] = {
                        x1, y2, z2, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                        x2, y2, z2, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                        x2, y1, z2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                        x1, y1, z2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                    };
                    GLuint indices[] = {
                        0u + num_coords, 1u + num_coords, 2u + num_coords,
                        0u + num_coords, 2u + num_coords, 3u + num_coords,
                    };

                    wall_vertices.insert(wall_vertices.end(), vertices, vertices + 32);
                    wall_indices.insert(wall_indices.end(), indices, indices + 6);
                    num_coords += 4;
                }
            }
        }

        std::vector<GLuint> attributes{ 3, 3, 2 };
        objects_.insert(std::pair<std::string, std::unique_ptr<moe::Object>>(
            "wall", std::make_unique<moe::Object>(wall_vertices, wall_indices,
                attributes)));
    }

    {  // Generate the pillars
        std::vector<GLfloat> pillar_vertices;
        std::vector<GLuint> pillar_indices;
        int num_coords = 0;

        for (int x = 0; x <= maze_col_; x++)
        {
            for (int y = 0; y <= maze_row_; y++)
            {
                GLfloat X = x * cell_width_;
                GLfloat Y = y * cell_width_;
                GLfloat W = wall_thickness_;

                GLfloat x1 = X - W, x2 = X + W;
                GLfloat y1 = 0.00f, y2 = cell_height_;
                GLfloat z1 = Y - W, z2 = Y + W;

                GLfloat vertices[] = { 
                    x1, y2, z1, -1.0f, 0.00f, 0.00f, 0.0f, 5.0f,
                    x1, y2, z2, -1.0f, 0.00f, 0.00f, 1.0f, 5.0f,
                    x1, y1, z2, -1.0f, 0.00f, 0.00f, 1.0f, 0.0f,
                    x1, y1, z1, -1.0f, 0.00f, 0.00f, 0.0f, 0.0f,
                    x1, y2, z2, 0.00f, 0.00f, 1.00f, 0.0f, 5.0f,
                    x2, y2, z2, 0.00f, 0.00f, 1.00f, 1.0f, 5.0f,
                    x2, y1, z2, 0.00f, 0.00f, 1.00f, 1.0f, 0.0f,
                    x1, y1, z2, 0.00f, 0.00f, 1.00f, 0.0f, 0.0f,
                    x2, y2, z2, 1.00f, 0.00f, 0.00f, 0.0f, 5.0f,
                    x2, y2, z1, 1.00f, 0.00f, 0.00f, 1.0f, 5.0f,
                    x2, y1, z1, 1.00f, 0.00f, 0.00f, 1.0f, 0.0f,
                    x2, y1, z2, 1.00f, 0.00f, 0.00f, 0.0f, 0.0f,
                    x2, y2, z1, 0.00f, 0.00f, -1.0f, 0.0f, 5.0f,
                    x1, y2, z1, 0.00f, 0.00f, -1.0f, 1.0f, 5.0f,
                    x1, y1, z1, 0.00f, 0.00f, -1.0f, 1.0f, 0.0f,
                    x2, y1, z1, 0.00f, 0.00f, -1.0f, 0.0f, 0.0f
                };

                GLuint indices[] = {
                    0u + num_coords,  1u + num_coords,  2u + num_coords,  0u + num_coords,
                    2u + num_coords,  3u + num_coords,  4u + num_coords,  5u + num_coords,
                    6u + num_coords,  4u + num_coords,  6u + num_coords,  7u + num_coords,
                    8u + num_coords,  9u + num_coords,  10u + num_coords, 8u + num_coords,
                    10u + num_coords, 11u + num_coords, 12u + num_coords, 13u + num_coords,
                    14u + num_coords, 12u + num_coords, 14u + num_coords, 15u + num_coords,
                };

                pillar_vertices.insert(pillar_vertices.end(), vertices, vertices + 128);
                pillar_indices.insert(pillar_indices.end(), indices, indices + 24);
                num_coords += 16;
            }
        }

        std::vector<GLuint> attributes{ 3, 3, 2 };
        objects_.insert(std::pair<std::string, std::unique_ptr<moe::Object>>(
            "pillar", std::make_unique<moe::Object>(pillar_vertices, pillar_indices,
                attributes)));
    }

    {  // Generate the ground
        std::vector<GLfloat> ground_vertices;
        std::vector<GLuint> ground_indices;
        int num_coords = 0;

        for (int x = 0; x < maze_col_; x++)
        {
            for (int y = 0; y < maze_row_; y++)
            {
                GLfloat X = x * cell_width_;
                GLfloat Y = y * cell_width_;
                GLfloat W = cell_width_;

                GLfloat x1 = X, x2 = X + W;
                GLfloat z1 = Y, z2 = Y + W;

                GLfloat vertices[] = {
                    x1, 0.0f, z1, 0.0f, 1.0f, 0.00f, 0.0f, 2.0f,
                    x2, 0.0f, z1, 0.0f, 1.0f, 0.00f, 2.0f, 2.0f,
                    x2, 0.0f, z2, 0.0f, 1.0f, 0.00f, 2.0f, 0.0f,
                    x1, 0.0f, z2, 0.0f, 1.0f, 0.00f, 0.0f, 0.0f,
                };

                GLuint indices[] = {
                    0u + num_coords, 1u + num_coords, 2u + num_coords,
                    0u + num_coords, 2u + num_coords, 3u + num_coords,
                };

                ground_vertices.insert(ground_vertices.end(), vertices, vertices + 32);
                ground_indices.insert(ground_indices.end(), indices, indices + 6);
                num_coords += 4;
            }
        }

        std::vector<GLuint> attributes{ 3, 3, 2 };
        objects_.insert(std::pair<std::string, std::unique_ptr<moe::Object>>(
            "ground", std::make_unique<moe::Object>(ground_vertices, ground_indices,
                attributes)));
    }

    {  // Configure the shader
        shaders_.at("flashlight")->Activate();

        // Set which texture unit to use
        shaders_.at("flashlight")->SetInt("material.diffuse", 0);

        shaders_.at("flashlight")
            ->SetFloat("light.innerCutOff", glm::cos(glm::radians(22.5f)));
        shaders_.at("flashlight")
            ->SetFloat("light.outerCutOff", glm::cos(glm::radians(27.5f)));

        // light properties
        shaders_.at("flashlight")->SetVec3("light.ambient", 0.1f, 0.1f, 0.1f);
        shaders_.at("flashlight")->SetVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        shaders_.at("flashlight")->SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
        shaders_.at("flashlight")->SetFloat("light.constant", 1.0f);
        shaders_.at("flashlight")->SetFloat("light.linear", 0.022f);
        shaders_.at("flashlight")->SetFloat("light.quadratic", 0.0019f);

        // world transformation
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat3 normal = glm::mat3(glm::transpose(glm::inverse(model)));
        shaders_.at("flashlight")->SetMat4("model", model);
        shaders_.at("flashlight")->SetMat3("normal", normal);
    }
}

void Labyrinth::ProcessMouseEvents()
{
    double xpos, ypos;
    glfwGetCursorPos(window_, &xpos, &ypos);

    float xoffset = (float) (xpos - last_mouse_x_);
    float yoffset = (float) (last_mouse_y_ - ypos);

    last_mouse_x_ = xpos;
    last_mouse_y_ = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void Labyrinth::ProcessKeyEvents()
{
    float curr_frame = (float) glfwGetTime();
    float delta_time = (float) (curr_frame - last_frame_);

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);

    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(kForward, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(kBackward, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(kLeft, delta_time);
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(kRight, delta_time);

    glm::vec3 p = camera.view_position_;
    int x = (int) (p.x / cell_width_);
    int y = (int) (p.z / cell_width_);
    int index = x + y * maze_col_;
    int cell = maze_.structure_.at(index);

    bool is_position_legal = false;
    if (fmod(p.x, cell_width_) < 0.2 * cell_width_)
    {
        if (fmod(p.z, cell_width_) < 0.8 * cell_width_ &&
            fmod(p.z, cell_width_) > 0.2 * cell_width_ && cell & kConnectsLeft)
        {
            is_position_legal = true;
        }
    }
    else if (fmod(p.x, cell_width_) > 0.8 * cell_width_)
    {
        if (fmod(p.z, cell_width_) < 0.8 * cell_width_ &&
            fmod(p.z, cell_width_) > 0.2 * cell_width_ && cell & kConnectsRight)
        {
            is_position_legal = true;
        }
    }
    else
    {
        if (fmod(p.z, cell_width_) < 0.8 * cell_width_ &&
            fmod(p.z, cell_width_) > 0.2 * cell_width_)
        {
            is_position_legal = true;
        }
        else if (fmod(p.z, cell_width_) > 0.8 * cell_width_ &&
            cell & kConnectsDown)
        {
            is_position_legal = true;
        }
        else if (fmod(p.z, cell_width_) < 0.2 * cell_width_ &&
            cell & kConnectsUp)
        {
            is_position_legal = true;
        }
    }

    if (is_position_legal)
    {
        position_ = camera.view_position_;
    }
    else
    {
        camera.view_position_ = position_;
    }

    last_frame_ = curr_frame;
}

void Labyrinth::RendorScene()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    shaders_.at("flashlight")->Activate();
    shaders_.at("flashlight")->SetVec3("light.position", position_);
    shaders_.at("flashlight")->SetVec3("light.direction", camera.view_front_);
    shaders_.at("flashlight")->SetVec3("viewPos", position_);

    // material properties
    shaders_.at("flashlight")->SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
    shaders_.at("flashlight")->SetFloat("material.shininess", 32.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)window_width_ / (float)window_height_, 0.1f,
        120.0f);
    shaders_.at("flashlight")->SetMat4("projection", projection);
    shaders_.at("flashlight")->SetMat4("view", camera.GetViewMatrix());

    textures_.at("wall")->Activate(0);
    objects_.at("wall")->Rendor();

    textures_.at("ground")->Activate(0);
    objects_.at("ground")->Rendor();

    textures_.at("pillar")->Activate(0);
    objects_.at("pillar")->Rendor();
}