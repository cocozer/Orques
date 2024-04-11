#include "random.hpp"
#include <cmath>

namespace randgen {

double Rand01() // renvoie un variable réelle et uniforme entre 0 et 1
{
    thread_local std::default_random_engine gen{std::random_device{}()};
    thread_local auto                       distrib = std::uniform_real_distribution<double>{0.0, 1.0};

    return distrib(gen);
}

double Poisson(double lambda) // renvoie un nombre d'évenement selon une loi de Poisson (parametre lambda = nombre moyen d'évènement par intervalle de temps)
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
    double M_PI   = 3.14159265358979323846;
    double z1     = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    double sample = esperance + ecarttype * z1;
    return sample;
}

} // namespace randgen