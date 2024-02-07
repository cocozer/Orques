#include "flock.hpp"

namespace boids {

Flock::Flock(int n)
{
    // Crée n boids avec des positions et des vitesses aléatoires, et les ajoute au vecteur de boids
    for (int i = 0; i < n; ++i)
    {
        float x  = 0;
        float y  = 0;
        float vx = 0;
        float vy = 0;
        flock.emplace_back(x, y, vx, vy);
    }
}

void Flock::AddBoid(const Boid& boid)
{
    flock.push_back(boid);
}

void Flock::RemoveBoid(int index)
{
    if (index >= 0 && index < flock.size())
    {
        flock.erase(flock.begin() + index);
    }
}

const Boid& Flock::GetBoid(int index) const
{
    return flock[index];
}

int Flock::NumBoids() const
{
    return flock.size();
}

void Flock::UpdatePositions()
{
    for (auto& boid : flock)
    {
        boid.UpdatePosition();
    }
}

// void Boids::UpdateVelocities() {
//     // À implémenter
// }

} // namespace boids