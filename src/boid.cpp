#include "boid.hpp"

Boid::Boid()
    : size(0.1), pos(0.0, 0.0), velocity(0.0, 0.0)
{}

Boid::Boid(const float& x, const float& y)
    : size(0.1), pos(x, y), velocity(0.0, 0.0)
{}

Boid::Boid(const float& x, const float& y, const float& vx, const float& vy)
    : size(0.1), pos(x, y), velocity(vx, vy)
{}

void Boid::UpdatePosition()
{
    pos += velocity;
}
