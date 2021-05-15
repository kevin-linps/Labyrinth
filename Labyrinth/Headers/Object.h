#ifndef MOE_OBJECT_H_
#define MOE_OBJECT_H_

#include <vector>

#include <glad/glad.h>

namespace moe
{
    class Object
    {
    protected:

        GLuint vao_, vbo_, ebo_, ebo_len_;

    public:

        Object(const std::vector<GLfloat>& vertices,
            const std::vector<GLuint>& indices,
            const std::vector<GLuint>& attributes);
        ~Object();

        void Rendor();
    };
}


#endif