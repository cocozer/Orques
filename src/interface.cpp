#include "interface.hpp"
#include <iostream>
#include <test_sphere.hpp>
#include <vector>
#include "flock.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

Interface::Interface()
    : ctx{{1280, 720, "Cher ImGui"}}, rayon_carre(0.3f), position_cercle(0, 0, 0), nombre_boids(10), taille_boids(0.03f), separation(0.01f), protected_range(0.1), alignement(0.05f), cohesion(0.001f), average_speed(0.01), turning_factor(0.01), fear_predator(0.001f), texte("Test")
{
    /*********************************
     * INITIALIZATION CODE
     *********************************/

    // Load shaders
    const p6::Shader shader = p6::load_shader("../shaders/3D.vs.glsl", "../shaders/normal.fs.glsl");

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16); // création des vertices de la sphere

    int    verticesSize = static_cast<int>(vertices.size());
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(verticesSize * sizeof(glimac::ShapeVertex)), vertices.data(), GL_STATIC_DRAW);
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

    ctx.imgui = [&]() {
        // Affiche une fenêtre simple
        ImGui::Begin("Test");
        ImGui::SliderFloat("Taille carrée", &rayon_carre, 0.f, 0.6f);
        ImGui::SliderFloat3("Position cercle", glm::value_ptr(position_cercle), 0.f, 1.f);
        ImGui::SliderInt("Nombre de Boids", &nombre_boids, 0, 50);
        if (ImGui::IsItemEdited())
        {
            setNumberOfBoids(nombre_boids);
        }
        ImGui::SliderFloat("Taille des Boids", &taille_boids, 0.015f, 0.1f);
        if (ImGui::IsItemEdited())
        {
            flock.UpdateBoidSize(taille_boids);
        }
        ImGui::SliderFloat("Separation", &separation, 0.0f, 0.03f);
        if (ImGui::IsItemEdited())
        {
            flock.setAvoidFactor(separation);
        }
        ImGui::SliderFloat("Protected Range", &protected_range, 0.05f, 0.2f);
        if (ImGui::IsItemEdited())
        {
            flock.setProtectedRange(protected_range);
        }
        ImGui::SliderFloat("Alignement", &alignement, 0, 0.05f, "%.5f", 0.00001f);
        if (ImGui::IsItemEdited())
        {
            flock.setAlignement(alignement);
        }
        ImGui::SliderFloat("Cohésion", &cohesion, 0, 0.005f, "%.5f", 0.00001f);
        if (ImGui::IsItemEdited())
        {
            flock.setCohesion(cohesion);
        }
        ImGui::SliderFloat("Average Speed", &average_speed, 0.0f, 0.02f);
        if (ImGui::IsItemEdited())
        {
            flock.setAverageSpeed(average_speed);
        }
        ImGui::SliderFloat("Turning Factor", &turning_factor, 0.0f, 0.01f);
        if (ImGui::IsItemEdited())
        {
            flock.setTurningFactor(turning_factor);
        }
        // avoid predators-------------------------------
        ImGui::SliderFloat("Avoid Predator", &fear_predator, 0.0f, 0.01f);
        if (ImGui::IsItemEdited())
        {
            flock.setAvoidPredator(fear_predator);
            for (const Boid& boid : flock.GetAllBoids())
            {
                std::cout << "Boid: " << (boid.getIsPredator() ? "Predator" : "Not Predator") << '\n';
            }
        }
        ImGui::InputText("Texte", &texte);

        ImGui::End();

        // Affiche la fenêtre de démonstration officielle d'ImGui
        ImGui::ShowDemoWindow();
    };

    ctx.update = [&]() {
        ctx.background({1, 0.5, 0.7, 1});
        ctx.square(p6::Center{}, p6::Radius{rayon_carre});
        flock.Update(rayon_carre);

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

        MVMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0., 0., 0.));
        // MVMatrix     = glm::rotate(MVMatrix, 0.f, glm::vec3(0, 1, 0));
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        glBindVertexArray(vao);

        flock.drawFlock3D(MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, verticesSize);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

void Interface::run_update_loop()
{
    setNumberOfBoids(10);
}

void Interface::setNumberOfBoids(int num)
{
    flock = boids::Flock(num);
    Boid predator;
    predator.setIsPredator(true);
    flock.AddBoid(predator);
}
