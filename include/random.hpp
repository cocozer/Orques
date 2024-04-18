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
#include <cmath>

namespace randgen {

struct Timer { // pour stocker le temps de début
    std::chrono::high_resolution_clock::time_point debut;
    Timer()
        : debut(std::chrono::high_resolution_clock::now()) {}
};

double Rand01();
long long int tempsEcoule(const Timer& timer);
int    Poisson(double lambda);

int    UniformeDouble(int min, int max);
double UniformeDouble(double min, double max);

double Normale(double esperance, double ecarttype);

glm::mat4 markovMat; // + il faut definir les probas
void markov_suivant(int actual_state, glm::vec4 v);
void chaine_markov(int actual_state);

} // namespace randgen
