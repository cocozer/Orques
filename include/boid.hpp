#pragma once

#include <vector>

class Boid {
private:
    float size;
    float x;
    float y;
    float vx;
    float vy;
    float ax;
    float ay;
    float u;

public:
    Boid();                                                                 // Default constructor
    Boid(const float& x, const float& y);                                   // Constructor by position
    Boid(const float& x, const float& y, const float& vx, const float& vy); // Constructor by position and velocity

    void UpdatePosition();
    // void UpdateVelocity();
};