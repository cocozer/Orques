#include "boid.hpp"
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

// changement
Boid::Boid()
    : size(0.4), pos(0.0, 0.0, -5.0f), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025), state(0)
{}

Boid::Boid(const float& x, const float& y, const float& z)
    : size(0.4), pos(x, y, z), velocity(0.0, 0.0, 0.0), state(0)
{}

Boid::Boid(float aspectRatio)
    : size(0.4), pos(p6::random::number(-aspectRatio, aspectRatio), p6::random::number(-1, 1), 0.0), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025), state(0)
{
}
Boid::Boid(int state)
    : size(0.4), pos(0.0, 0.0, -5.0f), velocity(randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025), state(state)
{}
void Boid::drawBoid(p6::Context& ctx) const
{
    if (getState() == 3)
    {
        ctx.use_stroke = true;
    }
    ctx.circle(p6::Center{this->pos.x, this->pos.y}, p6::Radius(this->size));
    ctx.use_stroke = false;
}
void Boid::drawBoid3D(glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, const Model& kw, std::vector<GLuint> bakesKw, GLint uTextureKw) const
{
    glm::mat4 ViewMatrixModel = glm::translate(glm::mat4(1.0), glm::vec3(getPos().x, getPos().y, getPos().z));

    if (getState() == 3)
    {
        ViewMatrixModel = glm::scale(ViewMatrixModel, glm::vec3(getSize() * 2));
    }
    else
    {
        ViewMatrixModel = glm::scale(ViewMatrixModel, glm::vec3(getSize()));
    }
    MVMatrix = MVMatrix * ViewMatrixModel;
    // on calcule la matrice de rotation pour orienter le boid dans la direction de sa vélocité
    glm::vec3 velocity       = getVel();
    float     angleY         = atan2(velocity.x, velocity.z);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f));

    // on applique la rotation à la matrice de modèle-vue
    MVMatrix     = MVMatrix * rotationMatrix;
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniform1i(uTextureKw, 0);
    // on bind les matrices au shader
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
    if (getState() == 1)
    {
        glBindTexture(GL_TEXTURE_2D, bakesKw[1]);
    }
    else if (getState() == 2)
    {
        glBindTexture(GL_TEXTURE_2D, bakesKw[2]);
    }
    else if (getState() == 3)
    {
        glBindTexture(GL_TEXTURE_2D, bakesKw[3]);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, bakesKw[0]);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // on dessine notre orque
    kw.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
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

void Boid::setState(int state)
{
    Boid::state = state;
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
    float coeff = 0.99f + (glm::abs(glm::max(glm::max(getVel().x, getVel().y), getVel().z))) * 75;
    if (pos.x < -2 * limit + 3 * size)
    {
        velocity.x += turnfactor * coeff;
    }
    if (pos.x > 2 * limit - 3 * size)
    {
        velocity.x -= turnfactor * coeff;
    }
    if (pos.y > 2 * limit - 3 * size)
    {
        velocity.y -= turnfactor * coeff;
    }
    if (pos.y < -2 * limit + 3 * size)
    {
        velocity.y += turnfactor * coeff;
    }
    if (pos.z > 2 * limit - 3 * size)
    {
        velocity.z -= turnfactor * coeff;
    }
    if (pos.z < -2 * limit + 3 * size)
    {
        velocity.z += turnfactor * coeff;
    }
}

int Boid::getState() const
{
    return state;
}
