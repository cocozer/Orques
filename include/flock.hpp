#pragma once

#include <vector>
#include "boid.hpp"

namespace boids {

class Flock {
private:
    std::vector<Boid> flock; // Vecteur de boids
    double            avoid_factor{0.004};
    double            matching_factor{0.005};
    double            centering_factor{0.0001};
    double            turn_factor{0.001};
    double            protected_range{0.05};
    double            visible_range{0.5};

public:
    // Flock();
    Flock(int n = 5);                                                                // Constructeur pour créer n boids
    Flock(int n, float avoid_factor, float matching_factor, float centering_factor); // Constructeur pour créer n boids avec un facteur d'évitement de "avoid_factor"

    // Méthodes pour manipuler le vecteur de boids
    void        AddBoid(const Boid& boid);
    void        RemoveBoid(int index);
    const Boid& GetBoid(int index);
    // const float GetTurnfactor() const;
    int NumBoids() const;

    void                     Update(float limit);
    void                     UpdateBoidSize(float newSize);
    const std::vector<Boid>& GetAllBoids();
    // méthode pour mettre à jour le nombre de boids
    void SetNumberOfBoids(int num);

private:
    // Méthodes pour mettre à jour tous les boids dans le vecteur
    void UpdatePositions();
    void MoveRandomly();
    void Separation();
    void Alignment();
    void Cohesion();
    void CheckOverflow(float limit);
    void assert_index_is_valid(int index);

    // void UpdateVelocities();
};
} // namespace boids