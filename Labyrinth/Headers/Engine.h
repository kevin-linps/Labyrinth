#ifndef MOE_ENGINE_H_
#define MOE_ENGINE_H_

#include <string>
#include <memory>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Texture.h"
#include "Object.h"

namespace moe
{
    class Engine
    {
    protected:

        std::map<std::string, std::unique_ptr<Shader>> shaders_;
        std::map<std::string, std::unique_ptr<Texture>> textures_;
        std::map<std::string, std::unique_ptr<Object>> objects_;

        GLFWwindow* window_;
        GLsizei window_width_;
        GLsizei window_height_;


        virtual void RendorScene();
        virtual void ProcessKeyEvents();
        virtual void ProcessMouseEvents();

        void ProcessWindowResize();
        void LoadSettingsFile(std::string file);

    public:

        Engine(GLFWwindow* window, std::string settings_file);

        virtual void Initialize();
        void Run();
    };

}


#endif