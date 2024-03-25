#define DOCTEST_CONFIG_IMPLEMENT
#include <p6/p6.h>
#include <interface.hpp>
#include "doctest/doctest.h"

// #define TINYOBJLOADER_IMPLEMENTATION
// #include <tiny_obj_loader.h>

int main()
{
    Interface interface; // on créé l'objet interface de la classe Interface

    // passer un model 3d a run update loop
    // ici definir model (shader, mv matrix etc et obj 3d)

    // load shader (creer shader vs fs)
    //     p6::Shader Shader = p6::load_shader("shaders/3D.vs.glsl", "shaders/text3D.fs.glsl");

    interface.run_update_loop();
    return 0;
}