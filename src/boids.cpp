#include "boids.hpp"

namespace boids {
Boid::Boid()
    : size(0.1), x(0.0), y(0.0), vx(0.0), vy(0.0)
{}

Boid::Boid(const float& x, const float& y)
    : size(0.1), x(x), y(y), vx(0.0), vy(0.0)
{}

Boid::Boid(const float& x, const float& y, const float& vx, const float& vy)
    : size(0.1), x(x), y(y), vx(vx), vy(vy)
{}

void Boid::UpdatePosition()
{
    x += vx;
    y += vy;
}

Boids::Boids(int n)
{
    // Crée n boids avec des positions et des vitesses aléatoires, et les ajoute au vecteur de boids
    for (int i = 0; i < n; ++i)
    {
        float x  = 0;
        float y  = 0;
        float vx = 0;
        float vy = 0;
        boids.emplace_back(x, y, vx, vy);
    }
}

void Boids::AddBoid(const Boid& boid)
{
    boids.push_back(boid);
}

void Boids::RemoveBoid(int index)
{
    if (index >= 0 && index < boids.size())
    {
        boids.erase(boids.begin() + index);
    }
}

const Boid& Boids::GetBoid(int index) const
{
    return boids[index];
}

int Boids::NumBoids() const
{
    return boids.size();
}

void Boids::UpdatePositions()
{
    for (auto& boid : boids)
    {
        boid.UpdatePosition();
    }
}

// void Boids::UpdateVelocities() {
//     // À implémenter
// }

} // namespace boids
