#include <iostream>
#include <vector>
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

int sphere_init(p6::Context& ctx)
{
    /*********************************
     * INITIALIZATION CODE
     *********************************/

    // Load shaders
    const p6::Shader shader = p6::load_shader("../shaders/3D.vs.glsl", "../shaders/normal.fs.glsl");

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16); // création des vertices de la sphere

    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vertices.size() * sizeof(glimac::ShapeVertex)), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);
    static constexpr GLuint vertex_attr_normal = 1;
    glEnableVertexAttribArray(vertex_attr_normal);
    static constexpr GLuint vertex_attr_color = 2;
    glEnableVertexAttribArray(vertex_attr_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position)); // attributs position : 3 coordonnées
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(vertex_attr_color, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glEnable(GL_DEPTH_TEST);
        // recuperation des matrices du shader
        GLint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
        GLint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
        GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f), 16.f / 9.f, 0.1f, 100.f);
        glm::mat4 MVMatrix;
        glm::mat4 NormalMatrix;

        MVMatrix     = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., -5.));
        MVMatrix     = glm::rotate(MVMatrix, 0.f, glm::vec3(0, 1, 0));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        // envoi des matrices vers le GPU
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertices.size()));

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}
