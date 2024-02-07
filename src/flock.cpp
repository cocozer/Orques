#include "flock.hpp"
#include <cassert>

namespace boids {

Flock::Flock(int n)
    : flock(n)
{
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
    else
    {
        assert(false);
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