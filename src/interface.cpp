#include "interface.hpp"
#include "flock.hpp"
#include "glm/gtc/type_ptr.hpp"

Interface::Interface()
    : ctx{{1280, 720, "Cher ImGui"}}, rayon_carre(0.5f), position_cercle(0, 0), nombre_boids(10), taille_boids(0.03f), separation(50), alignement(50), cohesion(50), texte("Test")
{
    ctx.imgui = [&]() {
        // Affiche une fenêtre simple
        ImGui::Begin("Test");
        ImGui::SliderFloat("Taille carrée", &rayon_carre, 0.f, 1.f);
        ImGui::SliderFloat2("Position cercle", glm::value_ptr(position_cercle), 0.f, 1.f);
        ImGui::SliderInt("Nombre de Boids", &nombre_boids, 0, 50);
        if (ImGui::IsItemEdited())
        {
            setNumberOfBoids(nombre_boids);
        }
        ImGui::SliderFloat("Taille des Boids", &taille_boids, 0.015f, 0.1f);
        ImGui::SliderInt("Séparation", &separation, 0, 100);
        ImGui::SliderInt("Alignement", &alignement, 0, 100);
        ImGui::SliderInt("Cohésion", &cohesion, 0, 100);
        ImGui::InputText("Texte", &texte);
        ImGui::End();

        // Affiche la fenêtre de démonstration officielle d'ImGui
        ImGui::ShowDemoWindow();
    };

    ctx.update = [&]() {
        ctx.background({1, 0.5, 0.7, 1});
        ctx.square(p6::Center{}, p6::Radius{rayon_carre});
        flock.Update(rayon_carre);

        ImGui::SliderFloat("Taille des Boids", &taille_boids, 0.001f, 0.0001f);
        flock.UpdateBoidSize(taille_boids);

        // for (const Boid& boid : flock.GetAllBoids())
        // {
        //     ctx.circle(p6::Center{boid.getPos()}, p6::Radius{boid.getSize()});
        // }
        flock.drawFlock(ctx);
    };
}

void Interface::run_update_loop()
{
    flock = boids::Flock(50);
    ctx.start();
}
void Interface::setNumberOfBoids(int num)
{
    flock = boids::Flock(num);
}