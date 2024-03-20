#include "flock.hpp"
#include <cassert>
#include "glm/fwd.hpp"
#include "random.hpp"

namespace boids {

// Flock::Flock()
//     : Flock(5)
// {
// }

Flock::Flock(int n)
    : flock(n)
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
} // brief draw boid in p6 window & param ctx the p6 context of creation

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
    if (index < 0 || index > static_cast<int>(flock.size()))
    {
        assert(false);
    }
}

// const float Flock::GetTurnfactor() const
// {
//     return turn_factor;
// }

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
        const glm::vec2 newVel = {randgen::Rand01() / 200 - 0.0025, randgen::Rand01() / 200 - 0.0025};
        boid.changeVelocity(newVel);
    }
}

void Flock::Separation()
{
    for (auto& boid : flock)
    {
        glm::vec2 close_d = {0, 0};
        for (const auto& other_boid : flock)
        {
            if (&boid == &other_boid)
                continue;

            float distance = glm::distance(boid.getPos(), other_boid.getPos());
            if (distance < protected_range)
            {
                close_d.x += boid.getPos().x - other_boid.getPos().x;
                close_d.y += boid.getPos().y - other_boid.getPos().y;
            }
        }

        glm::vec2 newVel;
        newVel.x = boid.getVel().x + (close_d.x * avoid_factor);
        newVel.y = boid.getVel().y + (close_d.y * avoid_factor);
        boid.changeVelocity(newVel);
    }
}

void Flock::Alignment()
{
    for (auto& boid : flock) // Pour chaque boid
    {
        glm::vec2 vel_avg           = {0, 0}; // On créé un vecteur pour la moyenne des vitesses des boids voisins
        float     neighboring_boids = 0;      // On créé un compteur pour les boids voisins
        for (const auto& other_boid : flock)  // Pour chaque boid voisin
        {
            if (&boid == &other_boid) //    Si le boid est lui-même
                continue;             // On passe au suivant

            const float distance = glm::distance(boid.getPos(), other_boid.getPos()); // On calcule la distance entre les deux boids
            if (distance < visible_range)                                             // Si la distance est inférieure à la portée de vision
            {
                vel_avg.x += other_boid.getVel().x; // On ajoute la vitesse du boid voisin à la moyenne
                vel_avg.y += other_boid.getVel().y; // On ajoute la vitesse du boid voisin à la moyenne
                neighboring_boids += 1;             // On incrémente le compteur de boids voisins
            }
        }
        if (neighboring_boids > 0) // Si il y a des boids voisins
        {
            vel_avg.x = vel_avg.x / neighboring_boids;                                                      // On divise la somme des vitesses par le nombre de boids voisins
            vel_avg.y = vel_avg.y / neighboring_boids;                                                      // On divise la somme des vitesses par le nombre de boids voisins
            glm::vec2 newVel;                                                                               // On créé un nouveau vecteur de vitesse
            newVel.x = boid.getVel().x + static_cast<float>(vel_avg.x - boid.getVel().x) * matching_factor; // On ajoute la différence entre la vitesse moyenne et la vitesse actuelle au vecteur de vitesse
            newVel.y = boid.getVel().y + static_cast<float>(vel_avg.y - boid.getVel().y) * matching_factor; // On ajoute la différence entre la vitesse moyenne et la vitesse actuelle au vecteur de vitesse
            boid.changeVelocity(newVel);                                                                    // On change la vitesse du boid
        }
    }
}

void Flock::Cohesion()
{
    for (auto& boid : flock)
    {
        glm::vec2 pos_avg           = {0, 0};
        float     neighboring_boids = 0;
        for (auto& other_boid : flock)
        {
            if (&boid != &other_boid)
            {
                float distance = glm::distance(boid.getPos(), other_boid.getPos());
                if (distance < visible_range)
                {
                    pos_avg.x += other_boid.getPos().x;
                    pos_avg.y += other_boid.getPos().y;
                    neighboring_boids += 1;
                }
            }
        }
        if (neighboring_boids > 0)
        {
            pos_avg.x = pos_avg.x / neighboring_boids;
            pos_avg.y = pos_avg.y / neighboring_boids;
            glm::vec2 newVel;
            newVel.x = boid.getVel().x + (pos_avg.x - boid.getPos().x) * centering_factor;
            newVel.y = boid.getVel().y + (pos_avg.y - boid.getPos().y) * centering_factor;
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

// void Boids::UpdateVelocities() {
//     // À implémenter
// }

void Flock::Update(float limit)
{
    // MoveRandomly();
    Separation();
    Alignment();
    Cohesion();
    CheckOverflow(limit);
    ClampSpeed();
    UpdatePositions();
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

} // namespace boids