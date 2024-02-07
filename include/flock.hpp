#pragma once

#include <vector>
#include "boid.hpp"

namespace boids {

class Flock {
private:
    std::vector<Boid> flock; // Vecteur de boids

public:
    Flock(int n); // Constructeur pour créer n boids

    // Méthodes pour manipuler le vecteur de boids
    void        AddBoid(const Boid& boid);
    void        RemoveBoid(int index);
    const Boid& GetBoid(int index) const;
    int         NumBoids() const;

    // Méthodes pour mettre à jour tous les boids dans le vecteur
    void UpdatePositions();
    // void UpdateVelocities();
};
} // namespace boids