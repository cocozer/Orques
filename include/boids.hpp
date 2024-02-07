#pragma once

#include <vector>

namespace boids {
class Boid {
private:
    float size;
    float x;
    float y;
    float vx;
    float vy;
    float ax;
    float ay;

public:
    Boid();                                                                 // Default constructor
    Boid(const float& x, const float& y);                                   // Constructor by position
    Boid(const float& x, const float& y, const float& vx, const float& vy); // Constructor by position and velocity

    void UpdatePosition();
    // void UpdateVelocity();
};

class Boids {
private:
    std::vector<Boid> boids; // Vecteur de boids

public:
    Boids(int n); // Constructeur pour créer n boids

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
