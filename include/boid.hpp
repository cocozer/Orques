#pragma once

#include <vector>
#include "glm/glm.hpp"

class Boid {
private:
    float     size;
    glm::vec2 pos;
    glm::vec2 velocity;
    glm::vec2 acceleration;

public:
    Boid();                                                                 // Default constructor
    Boid(const float& x, const float& y);                                   // Constructor by position
    Boid(const float& x, const float& y, const float& vx, const float& vy); // Constructor by position and velocity

    glm::vec2 getPos() const;
    glm::vec2 getVel() const;
    float     getSize() const;
    void      updatePosition();
    void      changeVelocity(glm::vec2 newVelocity);
    // void UpdateVelocity();
};