#include "interface.hpp"
#include "flock.hpp"

Interface::Interface()
    : ctx{{1280, 720, "Cher ImGui"}}, rayon_carre(0.5f), position_cercleX(0), position_cercleY(0), nombre_boids(10), taille_boids(0.5f), texte("Test")
{
    ctx.imgui = [&]() {
        // Affiche une fenêtre simple
        ImGui::Begin("Test");
        ImGui::SliderFloat("Taille carrée", &rayon_carre, 0.f, 1.f);
        ImGui::SliderFloat("Position cercle X", &position_cercleX, 0.f, 1.f);
        ImGui::SliderFloat("Position cercle Y", &position_cercleY, 0.f, 1.f);
        ImGui::SliderInt("Nombre de Boids", &nombre_boids, 0, 50);
        ImGui::SliderFloat("Taille des Boids", &taille_boids, 0.02f, 0.9f);
        ImGui::InputText("Texte", &texte);
        ImGui::End();

        // Affiche la fenêtre de démonstration officielle d'ImGui
        ImGui::ShowDemoWindow();
    };

    ctx.update = [&]() {
        ctx.background({1, 1, 0, 1});
        ctx.square(p6::Center{}, p6::Radius{rayon_carre});
        // flock.CheckOverflow(rayon_carre);
        flock.UpdatePositions();
        // flock.MoveRandomly();
        flock.Separation();
        flock.Alignment();

        for (Boid& boid : flock.GetAllBoids())
        {
            ctx.circle({boid.getPos().x, boid.getPos().y}, p6::Radius{boid.getSize()});
        }
    };
}

Interface::~Interface()
{
}

void Interface::afficher()
{
    flock = boids::Flock(3);
    ctx.start();
}
