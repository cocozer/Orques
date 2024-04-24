#include "random.hpp"
#include "glm/fwd.hpp"

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

glm::vec2 algue_pos(float taille_cube)
{
    double algue_x, algue_z;
    algue_x             = Normale(taille_cube / 2, taille_cube / 5);
    algue_z             = Normale(taille_cube / 2, taille_cube / 5);
    glm::vec2 algue_pos = {algue_x, algue_z};
    return algue_pos;
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

void changeBoidState(boids::Flock flock, p6::Context ctx)
{
    for (const Boid& boid : flock)
    {
        chaine_markov(boid, markovMat);
    }
}

} // namespace randgen