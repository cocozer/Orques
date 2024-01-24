#include <imgui.h>
#include <cstdlib>
#define DOCTEST_CONFIG_IMPLEMENT
#include <p6/p6.h>
#include "boids.hpp"
#include "doctest/doctest.h"
#include "p6/p6.h"

int main()
{
    auto        ctx           = p6::Context{{1280, 720, "Dear ImGui"}};
    auto        square_radius = 0.5f;
    auto        boids_number  = 10;
    auto        boids_size    = 0.5f;
    std::string text          = "Hello";
    ctx.imgui                 = [&]() {
        // Show a simple window
        ImGui::Begin("Test");
        ImGui::SliderFloat("Square size", &square_radius, 0.f, 1.f);
        ImGui::SliderInt("Boids number", &boids_number, 0, 50);
        ImGui::SliderFloat("Boids size", &boids_size, 0.02f, 0.9f);
        ImGui::InputText("Text", &text);
        ImGui::End();
        // Show the official ImGui demo window
        // It is very useful to discover all the widgets available in ImGui
        ImGui::ShowDemoWindow();
    };
    ctx.update = [&]() {
        ctx.background({1, 1, 0, 1});
        ctx.square(p6::Center{}, p6::Radius{square_radius});
        ctx.circle(p6::Center{}, p6::Radius{.1f});
    };
    ctx.start();
}