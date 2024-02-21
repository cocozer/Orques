#pragma once

#include <imgui.h>
#include <p6/p6.h>
#include <string>
#include "flock.hpp"

class Interface {
public:
    Interface();  // constructeur
    ~Interface(); // destructeur

    void afficher(); // fonction qui affiche l'interface graphique

private: // on déclare les variables membres de la classe Interface
    p6::Context  ctx;
    float        rayon_carre;
    float        position_cercleX;
    float        position_cercleY;
    int          nombre_boids;
    float        taille_boids;
    int separation = 50;
    int alignement = 50;
    int cohesion = 50;
    std::string  texte;
    boids::Flock flock;
};