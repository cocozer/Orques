#include "boids.hpp"


namespace boids {

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
