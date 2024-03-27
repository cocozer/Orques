#include "boid.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"
#include "random.hpp"

Boid::Boid()
    : size(0.02), pos(0.0, 0.0, 0.0), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025), isPredator(false)
{}

Boid::Boid(const float& x, const float& y, const float& z)
    : size(0.02), pos(x, y, z), velocity(0.0, 0.0, 0.0), isPredator(false)
{}

Boid::Boid(float aspectRatio)
    : size(0.2), pos(p6::random::number(-aspectRatio, aspectRatio), p6::random::number(-1, 1), 0.0), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025), isPredator(false)
{
}
Boid::Boid(bool isPredator)
    : size(0.02), pos(0.0, 0.0, 0.0), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025), isPredator(isPredator)
{}
void Boid::drawBoid(p6::Context& ctx) const
{
    if (getIsPredator())
    {
        ctx.use_stroke = true;
    }
    ctx.circle(p6::Center{this->pos.x, this->pos.y}, p6::Radius(this->size));
    ctx.use_stroke = false;
}
void Boid::drawBoid3D(glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix, glm::mat4 viewMatrix) const
{
    glm::mat4 MVMatrixBoids = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -3.f}); // Translation
    MVMatrixBoids           = glm::translate(MVMatrixBoids, getPos());          // Translation * Rotation * Translation

    MVMatrixBoids = glm::scale(MVMatrixBoids, glm::vec3{getSize()}); // Translation * Rotation * Translation * Scale
    MVMatrixBoids = viewMatrix * MVMatrixBoids;
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrixBoids));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrixBoids));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
}

Boid::Boid(const float& x, const float& y, const float& z, const float& vx, const float& vy, const float& vz)
    : size(0.02), pos(x, y, z), velocity(vx, vy, vz)
{}

glm::vec3 Boid::getPos() const
{
    return pos;
}

glm::vec3 Boid::getVel() const
{
    return velocity;
}

float Boid::getSize() const
{
    return size;
}

void Boid::setSize(float newSize)
{
    size = newSize;
}

void Boid::setIsPredator(bool isPredator)
{
    Boid::isPredator = isPredator;
}

void Boid::updatePosition()
{
    pos += velocity;
}

void Boid::changeVelocity(glm::vec3 newVelocity)
{
    velocity = newVelocity;
}

void Boid::clampSpeed(double max_speed, double min_speed)
{
    float currentSpeed = glm::length(velocity);

    if (currentSpeed > max_speed)
    {
        velocity = glm::normalize(velocity) * static_cast<float>(max_speed);
    }
    else if (currentSpeed < min_speed)
    {
        velocity = glm::normalize(velocity) * static_cast<float>(min_speed);
    }
}

void Boid::checkOverflow(float limit, float turnfactor)
{
    if (pos.x < -limit + 2 * size)
    {
        velocity.x += turnfactor;
    }
    if (pos.x > limit - 2 * size)
    {
        velocity.x -= turnfactor;
    }
    if (pos.y > limit - 2 * size)
    {
        velocity.y -= turnfactor;
    }
    if (pos.y < -limit + 2 * size)
    {
        velocity.y += turnfactor;
    }
}

bool Boid::getIsPredator() const
{
    return isPredator;
}
