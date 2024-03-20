#pragma once

#include <imgui.h>
#include <p6/p6.h>
#include <string>
#include "flock.hpp"

class Interface {
public:
    Interface(); // constructeur

    void run_update_loop(); // fonction qui affiche l'interface graphique
    void setNumberOfBoids(int num);

private: // on déclare les variables membres de la classe Interface
    p6::Context  ctx;
    float        rayon_carre;
    glm::vec2    position_cercle;
    int          nombre_boids;
    float        taille_boids;
    float        separation;
    float        protected_range;
    float        alignement;
    float        cohesion;
    float        average_speed;
    float        turning_factor;
    float        fear_predator;
    std::string  texte;
    boids::Flock flock;
};