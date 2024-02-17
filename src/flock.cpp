#include "flock.hpp"
#include <cassert>
#include "glm/fwd.hpp"
#include "random.hpp"

namespace boids {

Flock::Flock()
    : flock(5), avoid_factor(0.01)
{
}

Flock::Flock(int n)
    : flock(n), avoid_factor(0.01)
{
}

Flock::Flock(int n, float avoid_factor)
    : flock(n), avoid_factor(avoid_factor)
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
        boid.updatePosition();
    }
}

std::vector<Boid>& Flock::GetAllBoids()
{
    return flock;
}

void Flock::MoveRandomly()
{
    for (auto& boid : flock)
    {
        glm::vec2 newVel = {randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025};
        boid.changeVelocity(newVel);
    }
}

void Flock::Separation()
{
    for (auto& boid : flock)
    {
        glm::vec2 close_d = {0, 0};
        for (auto& other_boid : flock)
        {
            if (&boid != &other_boid)
            {
                close_d.x += boid.getPos().x - other_boid.getPos().x;
                close_d.y += boid.getPos().y - other_boid.getPos().y;
            }
        }
        glm::vec2 newVel;
        newVel.x = boid.getVel().x + close_d.x * avoid_factor;
        newVel.y = boid.getVel().y + close_d.y * avoid_factor;
        boid.changeVelocity(newVel);
    }
}

// void Boids::UpdateVelocities() {
//     // À implémenter
// }

} // namespace boids