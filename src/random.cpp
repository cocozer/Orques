#include "random.hpp"
#include <cmath>

namespace randgen {

double Rand01() // renvoie un variable réelle et uniforme entre 0 et 1
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
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

double Normale(double esperance, double ecarttype) // renvoie un double selon la loi Normale
                                                   // (méthode de Box-Muller)
{
    double u1     = Rand01();
    double u2     = Rand01();
    double z1     = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    double sample = esperance + ecarttype * z1;
    return sample;
}
} // namespace randgen