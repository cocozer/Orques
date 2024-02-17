#pragma once

#include <vector>
#include "boid.hpp"

namespace boids {

class Flock {
private:
    std::vector<Boid> flock; // Vecteur de boids
    float             avoid_factor;
    float             matching_factor;
    float             protected_range;
    float             visible_range;

public:
    Flock();
    Flock(int n);                                            // Constructeur pour créer n boids
    Flock(int n, float avoid_factor, float matching_factor); // Constructeur pour créer n boids avec un facteur d'évitement de "avoid_factor"

    // Méthodes pour manipuler le vecteur de boids
    void        AddBoid(const Boid& boid);
    void        RemoveBoid(int index);
    const Boid& GetBoid(int index) const;
    int         NumBoids() const;

    // Méthodes pour mettre à jour tous les boids dans le vecteur
    void               UpdatePositions();
    void               MoveRandomly();
    std::vector<Boid>& GetAllBoids();
    void               Separation();
    void               Alignment();
    void               CheckOverflow(float limit);
    // void UpdateVelocities();
};
} // namespace boids