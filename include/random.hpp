#pragma once
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include "glm/glm.hpp"
#include "flock.hpp"
#include "boid.hpp"
#include "model.hpp"

namespace randgen {

struct Timer { // pour stocker le temps de début
    std::chrono::high_resolution_clock::time_point debut;
    Timer()
        : debut(std::chrono::high_resolution_clock::now()) {}
};

double        Rand01();
long long int tempsEcoule(const Timer& timer);
int           Poisson(double lambda);

int    UniformeDouble(int min, int max);
double UniformeDouble(double min, double max);

double Exponentielle(double min, double max, double lambda);

double    Normale(double esperance, double ecarttype);
glm::vec3 algue_pos(float taille_cube);

// void markov_suivant(Boid& boid, glm::vec4 v);
// void chaine_markov(Boid& boid, glm::mat4 markovMat);
// void changeBoidState(boids::Flock flock, p6::Context ctx);

int initial_state = 0; 

glm::mat4 markovMat = glm::mat4(
    glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
    glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
    glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
    glm::vec4(0.10f, 0.10f, 0.10f, 0.70f)
);



} // namespace randgen
