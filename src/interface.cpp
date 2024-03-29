#include "interface.hpp"
#include <iostream>
#include "flock.hpp"
#include "glm/gtc/type_ptr.hpp"


Interface::Interface()
    : ctx{{1280, 720, "Cher ImGui"}}, rayon_carre(0.9f), position_cercle(0, 0, 0), nombre_boids(10), taille_boids(0.03f), separation(0.01f), protected_range(0.1), alignement(0.05f), cohesion(0.001f), average_speed(0.01), turning_factor(0.01), fear_predator(0.001f), texte("Test")
{
    ctx.imgui = [&]() {
        // Affiche une fenêtre simple
        ImGui::Begin("Test");
        ImGui::SliderFloat("Taille carrée", &rayon_carre, 0.f, 1.f);
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
        flock.drawFlock(ctx);
    };
}

void Interface::run_update_loop()
{
    setNumberOfBoids(20);
    ctx.start();
}

void Interface::setNumberOfBoids(int num)
{
    flock = boids::Flock(num);
    Boid predator;
    predator.setIsPredator(true);
    flock.AddBoid(predator);
}
