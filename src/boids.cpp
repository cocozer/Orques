#include "boids.hpp"

namespace boids {
Boids::Boids()
    : size(0.1), x(0.0), y(0.0), vx(0.0), vy(0.0)
{}

Boids::Boids(float x, float y)
    : size(0.1), x(x), y(y), vx(0.0), vy(0.0)
{}

Boids::Boids(float x, float y, float vx, float vy)
    : size(0.1), x(x), y(y), vx(vx), vy(vy)
{}
} // namespace boids