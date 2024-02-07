#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <imgui.h>
#include <p6/p6.h>
#include <string>

class Interface {
public:
    Interface();  // constructeur
    ~Interface(); // destructeur

    void afficher(); // fonction qui affiche l'interface graphique

private: // on déclare les variables membres de la classe Interface
    p6::Context ctx;
    float       rayon_carre;
    float       position_cercleX;
    float       position_cercleY;
    int         nombre_boids;
    float       taille_boids;
    std::string texte;
};

#endif // INTERFACE_HPP