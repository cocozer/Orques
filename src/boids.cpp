#include "boids.hpp"

namespace boids {
Boids::Boids()
    : size(0.1), x(0.0), y(0.0), vx(0.0), vy(0.0)
{}

Boids::Boids(const float& x, const float& y)
    : size(0.1), x(x), y(y), vx(0.0), vy(0.0)
{}

Boids::Boids(const float& x, const float& y, const float& vx, const float& vy)
    : size(0.1), x(x), y(y), vx(vx), vy(vy)
{}
} // namespace boids