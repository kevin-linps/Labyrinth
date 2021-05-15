#include "Engine.h"

#include <toml++/toml.h>
#include <toml++/toml_parser.h>

using namespace moe;

Engine::Engine(GLFWwindow* window, std::string settings_file)
{
    window_ = window;
    glfwGetWindowSize(window_, &window_width_, &window_height_);
    LoadSettingsFile(settings_file);
}

void Engine::Initialize() {}
void Engine::RendorScene() {}
void Engine::ProcessKeyEvents() {}
void Engine::ProcessMouseEvents() {}

void Engine::ProcessWindowResize()
{
    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    if (width != window_width_ || height != window_height_)
    {
        window_width_ = width;
        window_height_ = height;
        glViewport(0, 0, window_width_, window_height_);
    }
}

void Engine::LoadSettingsFile(std::string file)
{
    toml::parse_result settings = toml::parse_file(file.c_str());

    if (toml::table* tbl = settings["shaders"].as_table())
    {
        for (auto elem : *tbl)
        {
            std::string shader_name = elem.first;

            toml::table sub = *elem.second.as_table();
            std::optional<std::string> v = sub["vert"].value<std::string>();
            std::optional<std::string> f = sub["frag"].value<std::string>();

            if (v.has_value() && f.has_value())
            {
                shaders_.insert(std::pair<std::string, std::unique_ptr<Shader>>
                    (shader_name, std::make_unique<Shader>(v.value(), f.value())));
            }
        }
    }

    if (toml::table* tbl = settings["textures"].as_table())
    {
        for (auto elem : *tbl)
        {
            std::string texture_name = elem.first;
            std::optional<std::string> tf = elem.second.value<std::string>();

            if (tf.has_value())
            {
                textures_.insert(std::pair<std::string, std::unique_ptr<Texture>>
                    (texture_name, std::make_unique<Texture>(tf.value())));
            }
        }
    }
}

void Engine::Run()
{
    while (!glfwWindowShouldClose(window_))
    {
        ProcessWindowResize();
        ProcessMouseEvents();
        ProcessKeyEvents();

        RendorScene();

        glfwSwapBuffers(window_);
        glfwWaitEventsTimeout(0.01);
    }
}