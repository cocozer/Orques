#pragma once
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <thread>

namespace randgen {

double Rand01();
int UniformeDouble(int min, int max);
double UniformeDouble(double min, double max);
int Poisson(double lambda);
double Normale(double esperance, double ecarttype);

}
