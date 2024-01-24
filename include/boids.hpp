#pragma once

namespace boids {
class Boids {
private:
    float size;
    float x;
    float y;
    float vx;
    float vy;

public:
    Boids();                                                                 // Default constructor
    Boids(const float& x, const float& y);                                   // Constructor by position
    Boids(const float& x, const float& y, const float& vx, const float& vy); // Constructor by position and velocity
};
} // namespace boids