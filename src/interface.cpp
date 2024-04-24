#include "interface.hpp"
#include <Model.hpp>
#include <boid.hpp>
#include <iostream>
#include <vector>
#include "flock.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "light.hpp"
#include "p6/p6.h"
#include "random.hpp"
#include "surveyor.hpp"
#include "tiny_obj_loader.h"

Interface::Interface()
    : ctx{{1280, 720, "Cher ImGui"}}, rayon_cube(15.0f), nombre_boids(10), taille_boids(0.1f), separation(0.001f), alignement(0.00018f), cohesion(0.0002f), protected_range(3.5), visible_range(13), average_speed(0.01), turning_factor(0.005), fear_predator(0.001f), texte("Test")
{
    /*********************************
     * INITIALIZATION CODE
     *********************************/
    // on charge les shaders
    p6::Shader shader = p6::load_shader("../shaders/3D.vs.glsl", "../shaders/light.fs.glsl");

    // on charge les images
    img::Image img_kw      = p6::load_image_buffer("../assets/textures/Kw.png");
    img::Image img_kwBlue  = p6::load_image_buffer("../assets/textures/KwBlue.png");
    img::Image img_kwGreen = p6::load_image_buffer("../assets/textures/KwGreen.png");
    img::Image img_kwRed   = p6::load_image_buffer("../assets/textures/KwRed.png");
    img::Image img_water   = p6::load_image_buffer("../assets/textures/water_texture.png");
    img::Image img_turtle  = p6::load_image_buffer("../assets/textures/turtleBaked.png");
    img::Image img_alg     = p6::load_image_buffer("../assets/textures/alg_texture.png");

    Model kw = Model();
    kw.loadModel("kw.obj");

    Model skybox = Model();
    skybox.loadModel("cube.obj");

    Model turtle = Model();
    turtle.loadModel("arpenteur.obj");

    Model alg = Model();
    alg.loadModel("alg.obj");

    GLuint bakeTurtle = 0;
    glGenTextures(1, &bakeTurtle);
    glBindTexture(GL_TEXTURE_2D, bakeTurtle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_turtle.width(), img_turtle.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_turtle.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint bakeKw = 0;
    glGenTextures(1, &bakeKw);
    glBindTexture(GL_TEXTURE_2D, bakeKw);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_kw.width(), img_kw.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_kw.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint bakeKwBlue = 0;
    glGenTextures(1, &bakeKwBlue);
    glBindTexture(GL_TEXTURE_2D, bakeKwBlue);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_kw.width(), img_kw.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_kwBlue.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint bakeKwGreen = 0;
    glGenTextures(1, &bakeKwGreen);
    glBindTexture(GL_TEXTURE_2D, bakeKwGreen);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_kw.width(), img_kw.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_kwGreen.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint bakeKwRed = 0;
    glGenTextures(1, &bakeKwRed);
    glBindTexture(GL_TEXTURE_2D, bakeKwRed);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_kw.width(), img_kw.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_kwRed.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::vector<GLuint> bakesKw = {bakeKw, bakeKwBlue, bakeKwGreen, bakeKwRed};

    GLuint bakeSkybox = 0;
    glGenTextures(1, &bakeSkybox);
    glBindTexture(GL_TEXTURE_2D, bakeSkybox);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_water.width(), img_water.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_water.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint bakeAlg = 0;
    glGenTextures(1, &bakeAlg);
    glBindTexture(GL_TEXTURE_2D, bakeAlg);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_alg.width(), img_alg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_alg.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // on bind le vbo de l'orque 3D
    kw.setVbo();
    skybox.setVbo();
    turtle.setVbo();
    alg.setVbo();

    // on active le test de profondeur
    glEnable(GL_DEPTH_TEST);

    // on bind le vao de l'orque 3D
    kw.setVao();
    skybox.setVao();
    turtle.setVao();
    alg.setVao();

    // on initialise les matrices de transformation pour les shaders
    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    // on récupère les variables uniformes pour les shaders
    GLint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");

    // light
    GLint uKd              = glGetUniformLocation(shader.id(), "uKd");
    GLint uKs              = glGetUniformLocation(shader.id(), "uKs");
    GLint uShininess       = glGetUniformLocation(shader.id(), "uShininess");
    GLint uLightPos_vs     = glGetUniformLocation(shader.id(), "uLightPos_vs");
    GLint uLightIntensity  = glGetUniformLocation(shader.id(), "uLightIntensity");
    GLint uLightPos2_vs    = glGetUniformLocation(shader.id(), "uLightPos2_vs");
    GLint uLightIntensity2 = glGetUniformLocation(shader.id(), "uLightIntensity2");

    //  recup var uniforme texture
    GLint uTexture = glGetUniformLocation(shader.id(), "uText");
    if (uTexture == -1)
    {
        std::cerr << "Warning: Uniform uTexture not found in shader!" << '\n';
    }
    // on charge le modele 3D

    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
    Surveyor surveyor(turtle);
    bool     right = false;
    bool     left  = false;
    bool     up    = false;
    bool     down  = false;

    Light lightScene    = Light(glm::vec3{350.});
    Light lightSurveyor = Light(glm::vec3{0.01});

    ctx.imgui = [&]() {
        // Affiche une fenêtre simple
        ImGui::Begin("Test");
        ImGui::SliderInt("Nombre de Boids", &nombre_boids, 0, 50);
        if (ImGui::IsItemEdited())
        {
            setNumberOfBoids(nombre_boids);
        }
        ImGui::SliderFloat("Taille des Boids", &taille_boids, 0.1f, 0.4f);
        if (ImGui::IsItemEdited())
        {
            flock.UpdateBoidSize(taille_boids);
        }
        ImGui::SliderFloat("Separation", &separation, 0.0f, 0.01f);
        if (ImGui::IsItemEdited())
        {
            flock.setAvoidFactor(separation);
        }
        ImGui::SliderFloat("Alignement", &alignement, 0, 0.001f, "%.5f", 0.00001f);
        if (ImGui::IsItemEdited())
        {
            flock.setAlignement(alignement);
        }
        ImGui::SliderFloat("Cohésion", &cohesion, 0, 0.001f, "%.5f", 0.00001f);
        if (ImGui::IsItemEdited())
        {
            flock.setCohesion(cohesion);
        }
        ImGui::SliderFloat("Protected Range", &protected_range, 2.f, 4.f);
        if (ImGui::IsItemEdited())
        {
            flock.setProtectedRange(protected_range);
        }
        ImGui::SliderFloat("Visible Range", &visible_range, 7.f, 13.f);
        if (ImGui::IsItemEdited())
        {
            flock.setVisibleRange(visible_range);
        }
        ImGui::SliderFloat("Average Speed", &average_speed, 0.0f, 0.50f);
        if (ImGui::IsItemEdited())
        {
            flock.setAverageSpeed(average_speed);
        }
        ImGui::SliderFloat("Turning Factor", &turning_factor, 0.005f, 0.01f);
        if (ImGui::IsItemEdited())
        {
            flock.setTurningFactor(turning_factor);
        }
        // avoid predators-------------------------------
        ImGui::SliderFloat("Avoid Predator", &fear_predator, 0.0f, 5.0f);
        if (ImGui::IsItemEdited())
        {
            flock.setAvoidPredator(fear_predator);
        }
        ImGui::InputText("Texte", &texte);

        ImGui::End();

        // Affiche la fenêtre de démonstration officielle d'ImGui
        ImGui::ShowDemoWindow();
    };
    std::vector<glm::vec3> algPos    = randgen::generateAlgues(500, rayon_cube);
    std::vector<float>     algAngles = randgen::generateAngles(500);
    ctx.update                       = [&]() {
        ctx.background({1, 0.5, 0.7, 1});
        ctx.square(p6::Center{}, p6::Radius{rayon_cube});
        flock.Update(rayon_cube);

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        moveSurveyor(surveyor, left, right, up, down, ctx);
        MVMatrix = surveyor.getViewMatrix();

        surveyor.update_camera(MVMatrix);
        // config du shader
        shader.use();

        // light
        lightScene.drawLightScene(glm::vec3(0.f, 20.f, 10.f), ProjMatrix, MVMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix, shader, uKd, uKs, uShininess, uLightPos_vs, uLightIntensity);
        // lightScene2.drawLightScene(glm::vec3(0.f, 20.f, 15.f), ProjMatrix, MVMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix, shader, uKd, uKs, uShininess, uLightPos_vs, uLightIntensity);
        lightSurveyor.drawLightSurveyor(surveyor.getPosition(), ProjMatrix, MVMatrix, uMVPMatrix, uMVMatrix, uNormalMatrix, uLightPos2_vs, uLightIntensity2);

        //  on utilise le shader
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.drawModel(glm::vec3(0, 0, 0), glm::vec3(rayon_cube), MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, bakeSkybox, uTexture);
        flock.drawFlock3D(MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, kw, bakesKw, uTexture);
        surveyor.drawSurveyor(MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, turtle, bakeTurtle, uTexture, left, right);
        randgen::drawAlgues(algPos, algAngles, MVMatrix, uMVMatrix, uMVPMatrix, ProjMatrix, NormalMatrix, uNormalMatrix, bakeAlg, uTexture, alg);
        // on debind le vao
        glBindVertexArray(0);
    };
    // Should be done last. It starts the infinite loop.
    setNumberOfBoids(10);
    ctx.start();
}

void Interface::run_update_loop()
{
    setNumberOfBoids(10);
}

void Interface::setNumberOfBoids(int num)
{
    flock = boids::Flock(num);
    Boid bleu;
    bleu.setState(1);
    flock.AddBoid(bleu);
    Boid vert;
    vert.setState(2);
    flock.AddBoid(vert);
    Boid predator;
    predator.setState(3);
    flock.AddBoid(predator);
}
