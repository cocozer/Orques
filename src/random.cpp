#include "random.hpp"
#include "boid.hpp"
#include "flock.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "model.hpp"

namespace randgen {

double Rand01() // renvoie un variable réelle et uniforme entre 0 et 1
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

long long int tempsEcoule(const Timer& timer) // retourne le temps écoulé depuis le début (en millisecondes)
{
    auto current = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current - timer.debut)
        .count();
}

int Poisson(double lambda) // renvoie un nombre d'évenement selon une loi de Poisson (parametre lambda = nombre moyen d'évènement par intervalle de temps)
{
    double L = exp(-lambda);
    double p = 1.0;
    int    k = -1;
    do
    {
        k++;
        p *= Rand01();
    } while (p > L);
    return k;
}

int UniformeInt(int min, int max)
// renvoie un nombre d'évenement selon une loi uniforme, avec en
// paramètres les bornes de l'intervalle choisis par l'utilisateur (entier)
{
    int n1 = max - min + 1; // +1 pour inclure la borne max
    int n2 = n1 * Rand01();
    int n3 = n2 + min;
    return n3;
}

double UniformeDouble(double min, double max)
// renvoie un nombre d'évenement selon une loi uniforme, avec en
// paramètres les bornes de l'intervalle choisis par l'utilisateur (entier)
{
    double n1 = max - min;
    double n2 = n1 * Rand01();
    double n3 = n2 + min;
    return n3;
}

double Normale(double esperance, double ecarttype) // renvoie un double selon la loi Normale
                                                   // (méthode de Box-Muller)
{
    double u1     = Rand01();
    double u2     = Rand01();
    double z1     = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    double sample = esperance + ecarttype * z1;
    return sample;
}

glm::vec3 algue_pos(float taille_cube)
{
    double algue_x, algue_z;
    algue_x             = Normale(0, taille_cube * 4 / 5);
    algue_z             = Normale(0, taille_cube * 4 / 5);
    glm::vec3 algue_pos = {algue_x, -taille_cube * 2, algue_z};
    return algue_pos;
}

std::vector<glm::vec3> generateAlgues(int num_algue, float taille_cube)
{
    std::vector<glm::vec3> algPos;
    for (int i = 0; i < num_algue; i++)
    {
        glm::vec3 algue_pos = randgen::algue_pos(taille_cube);
        algPos.push_back(algue_pos);
    }
    return algPos;
}

std::vector<float> generateAngles(int num_algue)
{
    std::vector<float> algAngles;
    for (int i = 0; i < num_algue; i++)
    {
        float algue_angle = randgen::UniformeDouble(0.0, 2.0 * M_PI);
        algAngles.push_back(algue_angle);
    }
    return algAngles;
}

void drawAlgues(std::vector<glm::vec3> algPos, std::vector<float> algAngle, glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, GLuint bakedAlgue, GLint uTexture, Model& model)
{
    for (int i = 0; i < algPos.size(); i++)
    {
        model.drawModel(algPos[i], algAngle[i], glm::vec3(1), MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, bakedAlgue, uTexture);
    }
}

double Exponentielle(double min, double max, double lambda) // génération d'un float selon loi exponentielle avec la méthode de l'inverse de la fonction de répartition
{
    double u1 = Rand01();
    double u2 = -log(1 - u1) / lambda;
    return min + (max - min) * u2;
}

void markov_suivant(Boid& boid, glm::vec4 v)
{
    float a = Rand01();
    if (a < v[0])
    {
        boid.setState(0);
    }
    else if (a < v[0] + v[1])
    {
        boid.setState(1);
    }
    else if (a < v[0] + v[1] + v[2])
    {
        boid.setState(2);
    }
    else if (a < v[0] + v[1] + v[2] + v[3])
    {
        boid.setState(3);
    }
    else
    {
        boid.setState(4);
    }
}

void chaine_markov(Boid& boid, glm::mat4 markovMat)
{
    switch (boid.getState())
    {
    case 0:
        markov_suivant(boid, markovMat[0]);
        break;
    case 1:
        markov_suivant(boid, markovMat[1]);
        break;
    case 2:
        markov_suivant(boid, markovMat[2]);
        break;
    case 3:
        markov_suivant(boid, markovMat[3]);
        break;
    }
}

double Beta(double alpha, double beta, int taille_cube) // renvoie un double selon la loi Beta, avec
                                                        // l’algorithme de Jönk
{
    float a, b;
    do
    {
        double u1 = Rand01();
        double u2 = Rand01();
        a         = std::pow(u1, 1 / alpha);
        b         = std::pow(u2, 1 / beta);
    } while (a + b > 1);
    return (a / (a + b)) * (taille_cube * 4) - taille_cube * 2;
}

glm::vec3 chest_pos(float taille_cube)
{
    double chest_x, chest_z;
    chest_x             = Beta(2, 5, taille_cube);
    chest_z             = Beta(5, 1, taille_cube);
    glm::vec3 chest_pos = {chest_x, -taille_cube * 2 + 2, chest_z};
    return chest_pos;
}

bool Bernoulli(double p) { // loi de bernoulli, p étant la probabilité de succès, p=0.5 pour équilibre
    return Rand01() < p;
}

} // namespace randgen