#ifndef MOE_TEXTURE_H_
#define MOE_TEXTURE_H_

#include <string>

#include <glad/glad.h>

namespace moe
{
    class Texture
    {
    protected:

        GLuint id_;

    public:

        Texture(std::string file);

        void Activate(GLuint texture_unit);
    };
}


#endif