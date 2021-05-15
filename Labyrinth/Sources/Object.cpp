#include "Object.h"

using namespace moe;

Object::Object(
    const std::vector<GLfloat>& vertices,
    const std::vector<GLuint>& indices,
    const std::vector<GLuint>& attributes)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat),
        vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
        indices.data(), GL_STATIC_DRAW);
    ebo_len_ = indices.size();

    GLuint sum = 0, index = 0;
    for (GLuint stride : attributes)
        sum += stride;

    for (GLuint i = 0; i < attributes.size(); i++)
    {
        GLuint values = attributes.at(i);

        glVertexAttribPointer(i, values, GL_FLOAT, GL_FALSE, sum * sizeof(GLfloat),
            (void*)(index * sizeof(GLfloat)));
        glEnableVertexAttribArray(i);

        index += values;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Object::~Object()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

void Object::Rendor()
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, ebo_len_, GL_UNSIGNED_INT, 0);
}