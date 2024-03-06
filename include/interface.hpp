#pragma once

#include <imgui.h>
#include <p6/p6.h>
#include <string>
#include "flock.hpp"

class Interface {
public:
    Interface(); // constructeur

    void run_update_loop(); // fonction qui affiche l'interface graphique

private: // on déclare les variables membres de la classe Interface
    p6::Context  ctx;
    float        rayon_carre;
    glm::vec2    position_cercle;
    int          nombre_boids;
    float        taille_boids;
    int          separation;
    int          alignement;
    int          cohesion;
    std::string  texte;
    boids::Flock flock;
};