#include <imgui.h>
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include <p6/p6.h>
#include <interface.hpp>
#include "boids.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

int main()
{
    Interface interface; // on créé l'objet interface de la classe Interface
    interface.afficher();
    return 0;
}