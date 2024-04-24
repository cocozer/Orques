#define DOCTEST_CONFIG_IMPLEMENT
#include <p6/p6.h>
#include <interface.hpp>
#include "doctest/doctest.h"

int main()
{
    Interface interface;
    interface.run_update_loop();
    return 0;
}
