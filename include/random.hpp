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
#include "model.hpp"

class Boid;
class Flock;
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

double Beta(double alpha, double beta, int taille_cube);
bool   Bernoulli(double p);

double                 Normale(double esperance, double ecarttype);
glm::vec3              algue_pos(float taille_cube);
glm::vec3              chest_pos(float taille_cube);
glm::vec3              floatie_pos(float taille_cube);
std::vector<glm::vec3> generateAlgues(int num_algue, float taille_cube);
void                   drawAlgues(std::vector<glm::vec3> algPos, std::vector<float> algAngle, glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, GLuint bakedAlgue, GLint uTexture, Model& model);
std::vector<float>     generateAngles(int num_algue);
void                   markov_suivant(Boid& boid, glm::vec4 v);
void                   chaine_markov(Boid& boid, glm::mat4 markovMat);
void                   changeBoidState(Flock flock, p6::Context ctx);

// int initial_state = 0;

// glm::mat4 markovMat = glm::mat4(
//     glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
//     glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
//     glm::vec4(0.30f, 0.30f, 0.30f, 0.10f),
//     glm::vec4(0.10f, 0.10f, 0.10f, 0.70f)
// );

} // namespace randgen