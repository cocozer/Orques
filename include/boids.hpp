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
    Boids();                                     // Default constructor
    Boids(float x, float y);                     // Constructor by position
    Boids(float x, float y, float vx, float vy); // Constructor by position and velocity
};
} // namespace boids