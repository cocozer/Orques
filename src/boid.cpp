#include "boid.hpp"
#include "random.hpp"

Boid::Boid()
    : size(0.1), pos(0.0, 0.0), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025)
{}

Boid::Boid(const float& x, const float& y)
    : size(0.1), pos(x, y), velocity(0.0, 0.0)
{}

Boid::Boid(const float& x, const float& y, const float& vx, const float& vy)
    : size(0.1), pos(x, y), velocity(vx, vy)
{}

glm::vec2 Boid::getPos() const
{
    return pos;
}

glm::vec2 Boid::getVel() const
{
    return velocity;
}

float Boid::getSize() const
{
    return size;
}

void Boid::updatePosition()
{
    pos += velocity;
}
void Boid::changeVelocity(glm::vec2 newVelocity)
{
    velocity = newVelocity;
}

void Boid::checkOverflow(float limit)
{
    if (pos.x < -limit)
    {
        pos.x = 0; // teleportation to the center
    }
    else if (pos.x > limit)
    {
        pos.x = 0; // teleportation to the center
    }

    if (pos.y < -limit)
    {
        pos.y = 0; // teleportation to the center
    }
    else if (pos.y > limit)
    {
        pos.y = 0; // teleportation to the center
    }
}