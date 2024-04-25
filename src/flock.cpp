#include "flock.hpp"
#include <cassert>
#include "glm/fwd.hpp"
#include "random.hpp"

// blalalalala
namespace boids {

// Flock::Flock()
//     : Flock(5)
// {
// }

Flock::Flock(int n)
    : flock(n), poissonGen(randgen::Poisson(3))
{
}

Flock::Flock(int n, float avoid_factor, float matching_factor, float centering_factor)
    : flock(n), avoid_factor(avoid_factor), matching_factor(matching_factor), centering_factor(centering_factor)
{
}

void Flock::drawFlock(p6::Context& ctx) const
{
    for (const Boid& boid : flock)
    {
        boid.drawBoid(ctx);
    }
    // sphere(ctx);
} // brief draw boid in p6 window & param ctx the p6 context of creation

void Flock::drawFlock3D(glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, const Model& kw, std::vector<GLuint> bakesKw, GLint uTextureKw) const
{
    for (const Boid& boid : flock)
    {
        boid.drawBoid3D(MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, kw, bakesKw, uTextureKw);
    }
}

void Flock::AddBoid(const Boid& boid)
{
    flock.push_back(boid);
}

void Flock::RemoveBoid(int index)
{
    if (index >= 0 && index < flock.size())
    {
        flock.erase(flock.begin() + index);
    }
    else
    {
        assert(false);
    }
}

const Boid& Flock::GetBoid(int index)
{
    assert_index_is_valid(index);
    return flock[index];
}

void Flock::assert_index_is_valid(int index)
{
    if (index < 0 || index >= static_cast<int>(flock.size()))
    {
        assert(false);
    }
}

int Flock::NumBoids() const
{
    return static_cast<int>(flock.size());
}

void Flock::UpdatePositions()
{
    for (auto& boid : flock)
    {
        boid.updatePosition();
    }
}

const std::vector<Boid>& Flock::GetAllBoids()
{
    return flock;
}

void Flock::MoveRandomly()
{
    for (auto& boid : flock)
    {
        const glm::vec3 newVel = {
            randgen::Rand01() / 200 - 0.0025,
            randgen::Rand01() / 200 - 0.0025,
            randgen::Rand01() / 200 - 0.0025
        };
        boid.changeVelocity(newVel);
    }
}

void Flock::Separation()
{
    for (auto& boid : flock)
    {
        glm::vec3 close_d = {0, 0, 0};
        for (const auto& other_boid : flock)
        {
            if (&boid == &other_boid)
                continue;

            float distance = glm::distance(boid.getPos(), other_boid.getPos());
            if (distance < protected_range)
            {
                close_d.x += boid.getPos().x - other_boid.getPos().x;
                close_d.y += boid.getPos().y - other_boid.getPos().y;
                close_d.z += boid.getPos().z - other_boid.getPos().z;
            }
        }

        glm::vec3 newVel;
        newVel.x = boid.getVel().x + (close_d.x * avoid_factor);
        newVel.y = boid.getVel().y + (close_d.y * avoid_factor);
        newVel.z = boid.getVel().z + (close_d.z * avoid_factor);
        boid.changeVelocity(newVel);
    }
}

void Flock::Alignment()
{
    for (auto& boid : flock)
    {
        glm::vec3 vel_avg           = {0, 0, 0};
        float     neighboring_boids = 0;
        for (const auto& other_boid : flock)
        {
            if (&boid == &other_boid)
                continue;

            const float distance = glm::distance(boid.getPos(), other_boid.getPos());
            if (distance < visible_range)
            {
                vel_avg.x += other_boid.getVel().x;
                vel_avg.y += other_boid.getVel().y;
                vel_avg.z += other_boid.getVel().z;
                neighboring_boids += 1;
            }
        }
        if (neighboring_boids > 0)
        {
            vel_avg.x /= neighboring_boids;
            vel_avg.y /= neighboring_boids;
            vel_avg.z /= neighboring_boids;

            glm::vec3 newVel;
            newVel.x = boid.getVel().x + (vel_avg.x - boid.getVel().x) * matching_factor;
            newVel.y = boid.getVel().y + (vel_avg.y - boid.getVel().y) * matching_factor;
            newVel.z = boid.getVel().z + (vel_avg.z - boid.getVel().z) * matching_factor;

            boid.changeVelocity(newVel);
        }
    }
}
void Flock::Cohesion()
{
    for (auto& boid : flock)
    {
        glm::vec3 pos_avg           = {0, 0, 0};
        float     neighboring_boids = 0;
        for (const auto& other_boid : flock)
        {
            if (&boid != &other_boid)
            {
                float distance = glm::distance(boid.getPos(), other_boid.getPos());
                if (distance < visible_range)
                {
                    pos_avg.x += other_boid.getPos().x;
                    pos_avg.y += other_boid.getPos().y;
                    pos_avg.z += other_boid.getPos().z;
                    neighboring_boids += 1;
                }
            }
        }
        if (neighboring_boids > 0)
        {
            pos_avg.x /= neighboring_boids;
            pos_avg.y /= neighboring_boids;
            pos_avg.z /= neighboring_boids;

            glm::vec3 newVel;
            newVel.x = boid.getVel().x + (pos_avg.x - boid.getPos().x) * centering_factor;
            newVel.y = boid.getVel().y + (pos_avg.y - boid.getPos().y) * centering_factor;
            newVel.z = boid.getVel().z + (pos_avg.z - boid.getPos().z) * centering_factor;

            boid.changeVelocity(newVel);
        }
    }
}

void Flock::ClampSpeed()
{
    for (auto& boid : flock)
    {
        boid.clampSpeed(max_speed, min_speed);
    }
}

void Flock::CheckOverflow(float limit)
{
    for (auto& boid : flock)
    {
        boid.checkOverflow(limit, turn_factor);
    }
}

void Flock::ChangeStatesFlock()
{
    timeSinceLastState += 1;
    if (poissonGen == 0)
    {
        poissonGen = 1;
    }
    if (timeSinceLastState < stateChangeDuringTime * (3000 / poissonGen)) // Si on est dans la période où les boids peuvent changer d'état
    {
        return;
    }
    stateChangeDuringTime++;
    glm::mat4 markovMat = glm::mat4(
        glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
        glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
        glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
        glm::vec4(0.10f, 0.10f, 0.10f, 0.70f)
    );
    for (auto& boid : flock)
    {
        randgen::chaine_markov(boid, markovMat);
    }
    if (timeSinceLastState > 3000)
    {
        std::cout << timeSinceLastState << std::endl;
        timeSinceLastState    = 0;
        stateChangeDuringTime = 0;
        poissonGen            = randgen::Poisson(3);
    }
}

void Flock::Update(float limit)
{
    Separation();
    Alignment();
    Cohesion();
    CheckOverflow(limit);
    ClampSpeed();
    UpdatePositions();
    avoidPredators();
}

void Flock::UpdateBoidSize(float newSize)
{
    for (Boid& boid : flock)
    {
        boid.setSize(newSize);
    }
}
void Flock::SetNumberOfBoids(int num)
{
    flock.clear();
    for (int i = 0; i < num; ++i)
    {
        Boid newBoid;
        AddBoid(newBoid);
    }
}
// modifie le facteur d'eloignement
void Flock::setAvoidFactor(float factor)
{
    avoid_factor = static_cast<double>(factor);
}

void Flock::setProtectedRange(float factor)
{
    protected_range = static_cast<double>(factor);
}
void Flock::setVisibleRange(float factor)
{
    visible_range = static_cast<double>(factor);
}

void Flock::setAlignement(float factor)
{
    matching_factor = static_cast<double>(factor);
}

void Flock::setCohesion(float factor)
{
    centering_factor = static_cast<double>(factor);
}

void Flock::setAverageSpeed(float factor)
{
    min_speed = static_cast<double>(factor) - 0.0025;
    max_speed = static_cast<double>(factor) + 0.0025;
}

void Flock::setTurningFactor(float factor)
{
    turn_factor = static_cast<double>(factor) - 0.0025;
}
void Flock::setAvoidPredator(float fear_predator)
{
    this->fear_predator = static_cast<double>(fear_predator);
}

void Flock::avoidPredators()
{
    for (auto& boid : flock)
    {
        for (const auto& other_boid : flock)
        {
            if (&boid == &other_boid)
                continue;

            if (other_boid.getState() == 3)
            {
                float distance = glm::distance(boid.getPos(), other_boid.getPos());
                if (distance < visible_range)
                {
                    glm::vec3 avoid_d = boid.getPos() - other_boid.getPos();
                    glm::vec3 newVel  = boid.getVel() + (avoid_d * static_cast<float>(fear_predator));
                    boid.changeVelocity(newVel);
                }
            }
        }
    }
}

} // namespace boids