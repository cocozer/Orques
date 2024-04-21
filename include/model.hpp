#pragma once

#include <glimac/common.hpp>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Model {
protected:
    GLuint                           m_vbo = 0;
    GLuint                           m_vao = 0;
    std::vector<glimac::ShapeVertex> m_vertices;
    std::vector<int>                 m_index;
    GLsizei                          m_vertexCount = 0;

public:
    Model() = default;

    ~Model()
    {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    };
    void drawModel(glm::vec3 position, glm::vec3 scale, glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, GLuint bakedTexture, GLint uTextureName);
    void draw() const
    {
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
    }
    GLuint getVao() const
    {
        return m_vao;
    }
    GLuint getVbo() const
    {
        return m_vbo;
    }
    void setVao();

    void setVbo();

    const glimac::ShapeVertex* getDataPointer() const;

    GLsizei getVertexCount() const
    {
        return m_vertexCount;
    }

    const int* getIndexPointer() const;

    void loadModel(const std::string& fileName);
};