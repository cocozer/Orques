#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "model.hpp"
#include "p6/p6.h"

class Boid {
private:
    float     size;
    glm::vec3 pos;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    bool      isPredator;

public:
    Boid();
    Boid(const float& x, const float& y, const float& z);
    Boid(const float& x, const float& y, const float& z, const float& vx, const float& vy, const float& vz);
    explicit Boid(bool isPredator);

    explicit Boid(float aspectRatio);      // brief constructor random & param aspectRatio window ratio
    void drawBoid(p6::Context& ctx) const; // brief draw boid in p6 window & param ctx the p6 context of creation
    void drawBoid3D(glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, Model boidModel) const;

    glm::vec3 getPos() const;
    glm::vec3 getVel() const;
    float     getSize() const;
    void      setSize(float newSize);
    void      setIsPredator(bool isPredator);
    void      updatePosition();
    void      changeVelocity(glm::vec3 newVelocity);
    void      clampSpeed(double max_speed, double min_speed);
    void      checkOverflow(float limit, float turnfactor);
    bool      getIsPredator() const;
};