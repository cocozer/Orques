#pragma once

namespace boids {
class Boids {
private:
    float size;
    float x;
    float y;
    float vx;
    float vy;
    float ax;
    float ay;

public:
    Boids();                                                                 // Default constructor
    Boids(const float& x, const float& y);                                   // Constructor by position
    Boids(const float& x, const float& y, const float& vx, const float& vy); // Constructor by position and velocity

    void UpdatePosition();
    void UpdateVelocity();

};
} // namespace boids