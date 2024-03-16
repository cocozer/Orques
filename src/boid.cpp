#include "boid.hpp"
#include "p6/p6.h"
#include "random.hpp"

Boid::Boid()
    : size(0.02), pos(0.0, 0.0), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025)
{}

Boid::Boid(const float& x, const float& y)
    : size(0.02), pos(x, y), velocity(0.0, 0.0)
{}

Boid::Boid(float aspectRatio)
    : size(0.2), pos(p6::random::number(-aspectRatio, aspectRatio), p6::random::number(-1, 1)), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025)
{
}

void Boid::drawBoid(p6::Context& ctx) const
{
    ctx.circle(p6::Center{this->pos.x, this->pos.y}, p6::Radius(this->size));
    ctx.use_stroke = false;
}

Boid::Boid(const float& x, const float& y, const float& vx, const float& vy)
    : size(0.02), pos(x, y), velocity(vx, vy)
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

void Boid::setSize(float newSize)
{
    size = newSize;
}

void Boid::updatePosition()
{
    pos += velocity;
}
void Boid::changeVelocity(glm::vec2 newVelocity)
{
    velocity = newVelocity;
}

void Boid::clampSpeed(double max_speed, double min_speed)
{
    float currentSpeed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    if (currentSpeed > max_speed)
    {
        velocity.x = (velocity.x / currentSpeed) * max_speed;
        velocity.y = (velocity.y / currentSpeed) * max_speed;
    }
    else if (currentSpeed < min_speed)
    {
        velocity.x = (velocity.x / currentSpeed) * min_speed;
        velocity.y = (velocity.y / currentSpeed) * min_speed;
    }
}
void Boid::checkOverflow(float limit, float turnfactor)
{
    if (pos.x < -limit + 2 * size)
    {
        velocity.x += turnfactor;
    }
    if (pos.x > limit - 2 * size)
    {
        velocity.x -= turnfactor;
    }
    if (pos.y > limit - 2 * size)
    {
        velocity.y -= turnfactor;
    }
    if (pos.y < -limit + 2 * size)
    {
        velocity.y += turnfactor;
    }
}