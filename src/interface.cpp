#include "interface.hpp"
#include <Model.hpp>
#include <boid.hpp>
#include <iostream>
#include <vector>
#include "flock.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include "tiny_obj_loader.h"

// blalalalalallalall
Interface::Interface()
    : ctx{{1280, 720, "Cher ImGui"}}, rayon_carre(0.3f), position_cercle(0, 0, 0), nombre_boids(10), taille_boids(0.03f), separation(0.01f), protected_range(0.1), alignement(0.05f), cohesion(0.001f), average_speed(0.01), turning_factor(0.01), fear_predator(0.001f), texte("Test")
{
    /*********************************
     * INITIALIZATION CODE
     *********************************/

    // on charge les shaders
    p6::Shader shader = p6::load_shader("../shaders/3D.vs.glsl", "../shaders/normal.fs.glsl");

    // on récupère les variables uniformes pour les shaders
    GLint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

    // on charge le modele 3D
    Model kw = Model();
    kw.loadModel("kw.obj");
    // on bind le vbo de l'orque 3D
    kw.setVbo();

    // on active le test de profondeur
    glEnable(GL_DEPTH_TEST);

    // on bind le vao de l'orque 3D
    kw.setVao();

    // on initialise les matrices de transformation pour les shaders
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);

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
        // config du shader
        shader.use();

        // glClearColor(0.9f, 0.1f, 0.6f, 1.f);

        // on utilise le shader
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // flock.drawFlock3D(MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, kw);
        for (const Boid& boid : flock.GetAllBoids())
        {
            // on calcule les matrices de vue et normales
            MVMatrix = glm::translate(glm::mat4(1.0), glm::vec3(boid.getPos().x, boid.getPos().y, boid.getPos().z - 1.));
            MVMatrix = glm::scale(MVMatrix, glm::vec3(boid.getSize())); // Scale the model matrix to the size of the sphere

            // on calcule la matrice de rotation pour orienter le boid dans la direction de sa vélocité
            glm::vec3 velocity       = boid.getVel();
            float     angleY         = atan2(velocity.x, velocity.z);
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f));

            // on applique la rotation à la matrice de modèle-vue
            MVMatrix     = MVMatrix * rotationMatrix;
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

            // on bind les matrices au shader
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            // on dessine notre orque
            kw.draw();
        }

        // on debind le vao
        glBindVertexArray(0);
    };
    // Should be done last. It starts the infinite loop.
    ctx.start();
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
