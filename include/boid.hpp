#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "p6/p6.h"

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

    explicit Boid(float aspectRatio);      // brief constructor random & param aspectRatio window ratio
    void drawBoid(p6::Context& ctx) const; // brief draw boid in p6 window & param ctx the p6 context of creation

    glm::vec2 getPos() const;
    glm::vec2 getVel() const;
    float     getSize() const;
    void      setSize(float newSize);
    void      updatePosition();
    void      changeVelocity(glm::vec2 newVelocity);
    void      checkOverflow(float limit, float turnfactor); // Check if the boid is outside the window, if he is, invert position

    // void UpdateVelocity();
};