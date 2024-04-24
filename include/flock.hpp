#pragma once

#include <vector>
#include "boid.hpp"
#include "model.hpp"
#include "random.hpp"
#include "test_sphere.hpp"

namespace boids {
class Flock {
private:
    std::vector<Boid> flock;                    // Vecteur de boids
    double            avoid_factor{0.001};      // separation
    double            matching_factor{0.00018}; // alignement
    double            centering_factor{0.0002}; // cohesion
    double            turn_factor{0.005};
    double            protected_range{3.5};
    double            visible_range{13.0};
    double            min_speed{0.05};
    double            max_speed{0.1};
    double            fear_predator{0.01};

public:
    // Flock();
    Flock(int n = 10);                                                               // Constructeur pour créer n boids
    Flock(int n, float avoid_factor, float matching_factor, float centering_factor); // Constructeur pour créer n boids avec un facteur d'évitement de "avoid_factor"

    void drawFlock(p6::Context& ctx) const; // brief draw boid in p6 window & param ctx the p6 context of creation
    void drawFlock3D(glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, const Model& kw, std::vector<GLuint> bakesKw, GLint uTextureKw) const;

    // Méthodes pour manipuler le vecteur de boids
    void        AddBoid(const Boid& boid);
    void        RemoveBoid(int index);
    const Boid& GetBoid(int index);
    // const float GetTurnfactor() const;
    int NumBoids() const;

    void                     Update(float limit);
    void                     UpdateBoidSize(float newSize);
    const std::vector<Boid>& GetAllBoids();
    // méthode pour mettre à jour le nombre de boids
    void SetNumberOfBoids(int num);
    // facteur d'evitement :
    void setAvoidFactor(float factor);
    // protection range :
    void setProtectedRange(float factor);
    void setVisibleRange(float factor);
    void setAlignement(float factor);
    void setCohesion(float factor);
    void setAverageSpeed(float factor);
    void setTurningFactor(float factor);
    void setAvoidPredator(float fear_predator);

private:
    // Méthodes pour mettre à jour tous les boids dans le vecteur
    void UpdatePositions();
    void MoveRandomly();
    void Separation();
    void Alignment();
    void Cohesion();
    void ClampSpeed();
    void CheckOverflow(float limit);
    void assert_index_is_valid(int index);
    void avoidPredators();

    // void UpdateVelocities();
};
} // namespace boids
