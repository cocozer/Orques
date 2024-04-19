#include "boid.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

// blalalalala
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
void Boid::drawBoid3D(glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, const Model& kw) const
{
    // on calcule les matrices de vue et normales
    MVMatrix = glm::translate(glm::mat4(1.0), glm::vec3(getPos().x, getPos().y, getPos().z - 1.));
    MVMatrix = glm::scale(MVMatrix, glm::vec3(getSize())); // Scale the model matrix to the size of the sphere

    // on calcule la matrice de rotation pour orienter le boid dans la direction de sa vélocité
    glm::vec3 velocity       = getVel();
    float     angleY         = atan2(velocity.x, velocity.z);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f));

    // on applique la rotation à la matrice de modèle-vue
    MVMatrix     = MVMatrix * rotationMatrix;
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    // on bind les matrices au shader
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    // on dessine notre orque
    kw.draw();
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
    if (pos.z > limit - 2 * size)
    {
        velocity.z -= turnfactor;
    }
    if (pos.z < -limit + 2 * size)
    {
        velocity.z += turnfactor;
    }
}

bool Boid::getIsPredator() const
{
    return isPredator;
}