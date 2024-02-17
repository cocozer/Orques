#include "flock.hpp"
#include <cassert>
#include "glm/fwd.hpp"
#include "random.hpp"

namespace boids {

Flock::Flock()
    : flock(5), avoid_factor(0.001), matching_factor(0.005), centering_factor(0.0001), turn_factor(0.001), protected_range(0.05), visible_range(0.5)
{
}

Flock::Flock(int n)
    : flock(n), avoid_factor(0.001), matching_factor(0.005), centering_factor(0.0001), turn_factor(0.001), protected_range(0.05), visible_range(0.5)
{
}

Flock::Flock(int n, float avoid_factor, float matching_factor, float centering_factor)
    : flock(n), avoid_factor(avoid_factor), matching_factor(matching_factor), centering_factor(centering_factor), protected_range(0.01), visible_range(0.5)
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

const float Flock::GetTurnfactor() const
{
    return turn_factor;
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
                float distance = glm::distance(boid.getPos(), other_boid.getPos());
                if (distance < protected_range)
                {
                    close_d.x += boid.getPos().x - other_boid.getPos().x;
                    close_d.y += boid.getPos().y - other_boid.getPos().y;
                }
            }
        }
        glm::vec2 newVel;
        newVel.x = boid.getVel().x + close_d.x * avoid_factor;
        newVel.y = boid.getVel().y + close_d.y * avoid_factor;
        boid.changeVelocity(newVel);
    }
}

void Flock::Alignment()
{
    for (auto& boid : flock)
    {
        glm::vec2 vel_avg           = {0, 0};
        float     neighboring_boids = 0;
        for (auto& other_boid : flock)
        {
            if (&boid != &other_boid)
            {
                float distance = glm::distance(boid.getPos(), other_boid.getPos());
                if (distance < visible_range)
                {
                    vel_avg.x += other_boid.getVel().x;
                    vel_avg.y += other_boid.getVel().y;
                    neighboring_boids += 1;
                }
            }
        }
        if (neighboring_boids > 0)
        {
            vel_avg.x = vel_avg.x / neighboring_boids;
            vel_avg.y = vel_avg.y / neighboring_boids;
            glm::vec2 newVel;
            newVel.x = boid.getVel().x + (vel_avg.x - boid.getVel().x) * matching_factor;
            newVel.y = boid.getVel().y + (vel_avg.y - boid.getVel().y) * matching_factor;
            boid.changeVelocity(newVel);
        }
    }
}

void Flock::Cohesion()
{
    for (auto& boid : flock)
    {
        glm::vec2 pos_avg           = {0, 0};
        float     neighboring_boids = 0;
        for (auto& other_boid : flock)
        {
            if (&boid != &other_boid)
            {
                float distance = glm::distance(boid.getPos(), other_boid.getPos());
                if (distance < visible_range)
                {
                    pos_avg.x += other_boid.getPos().x;
                    pos_avg.y += other_boid.getPos().y;
                    neighboring_boids += 1;
                }
            }
        }
        if (neighboring_boids > 0)
        {
            pos_avg.x = pos_avg.x / neighboring_boids;
            pos_avg.y = pos_avg.y / neighboring_boids;
            glm::vec2 newVel;
            newVel.x = boid.getVel().x + (pos_avg.x - boid.getPos().x) * centering_factor;
            newVel.y = boid.getVel().y + (pos_avg.y - boid.getPos().y) * centering_factor;
            boid.changeVelocity(newVel);
        }
    }
}

void Flock::CheckOverflow(float limit, float turnfactor)
{
    for (auto& boid : flock)
    {
        boid.checkOverflow(limit, turnfactor);
    }
}

// void Boids::UpdateVelocities() {
//     // À implémenter
// }

} // namespace boids