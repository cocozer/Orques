#include "surveyor.hpp"
#include <p6/p6.h>
#include <glm/glm.hpp>
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/trigonometric.hpp"

//---Camera---
Surveyor::Surveyor(Model& model)
    : m_cam_position(glm::vec3(0., -1., 0.)), m_phi(p6::PI), m_theta(0.), m_rotation_angle(0.), m_model(model), m_cam_distance(0.5f), m_cam_height(0.2f)
{
    this->setDirectionVectors();
}

void Surveyor::setDirectionVectors()
{
    // Calcul des vecteurs de direction en fonction des angles phi et theta
    m_front_vector = glm::vec3(glm::cos(m_theta) * glm::sin(m_phi), glm::sin(m_theta), glm::cos(m_theta) * glm::cos(m_phi));
    m_left_vector  = glm::vec3(glm::sin(m_phi + (p6::PI / 2)), 0, glm::cos(m_phi + (p6::PI / 2)));
    m_up_vector    = glm::cross(m_front_vector, m_left_vector);
    // std::cout << "Frontvector : " << m_front_vector .x << "+" << m_front_vector .y << "+" << m_front_vector .z << std::endl;
    // std::cout << "LeftVector : " << m_left_vector  .x << "+" << m_left_vector  .y << "+" << m_left_vector  .z << std::endl;
    // std::cout << "UpVector : " << m_up_vector    .x << "+" << m_up_vector    .y << "+" << m_up_vector    .z << std::endl;
    // std::cout << "Position : " << m_Position.x << "+" << m_Position.y << "+" << m_Position.z << std::endl;
    // std::cout << "-------------------------" << std::endl;
}

void Surveyor::setPosition(glm::vec3 position_surveyor)
{
    // Définition de la position de la caméra
    m_cam_position = position_surveyor;
}
glm::vec3 Surveyor::getPosition()
{
    // Récupération de la position de la caméra
    return m_cam_position;
}
void Surveyor::setRotationAngle(float rotation_angle)
{
    // Définition de l'angle de rotation de la caméra
    m_rotation_angle = rotation_angle;
}
float Surveyor::getRotationAngle() const
{
    // Récupération de l'angle de rotation de la caméra
    return m_rotation_angle;
}
void Surveyor::moveToLeft(float t)
{
    // Déplacement de la caméra vers la gauche
    m_cam_position += t * m_left_vector;
    setDirectionVectors();
}
void Surveyor::moveToFront(float t)
{
    // Déplacement de la caméra vers l'avant
    m_cam_position += t * m_front_vector;
    setDirectionVectors();
}
void Surveyor::rotateToLeft(float degrees)
{
    // Rotation de la caméra vers la gauche
    m_phi += glm::radians(degrees);
    setDirectionVectors();
}
void Surveyor::rotateToUp(float degrees)
{
    // Rotation de la caméra vers le haut
    m_theta += glm::radians(degrees);
    setDirectionVectors();
}
glm::mat4 Surveyor::getViewMatrix() const
{
    // Récupération de la matrice de vue de la caméra
    return glm::lookAt(m_cam_position, m_cam_position + m_front_vector, m_up_vector);
}
void Surveyor::update_camera(glm::mat4& ViewMatrix)
{
    // Mise à jour de la position de la caméra en fonction de la vue du surveyor
    glm::mat4 modelRotationMatrix = glm::rotate(glm::mat4(1.0f), getRotationAngle(), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 invPlayerViewMatrix = glm::inverse(ViewMatrix);
    auto      surveyorPosition    = glm::vec3(invPlayerViewMatrix[3]);
    glm::vec3 cameraPos           = surveyorPosition + m_cam_distance * glm::vec3(modelRotationMatrix * glm::vec4(invPlayerViewMatrix[2])) + m_cam_height * glm::vec3(invPlayerViewMatrix[1]);
    ViewMatrix                    = glm::lookAt(cameraPos, surveyorPosition, glm::vec3(0.0f, 1.0f, 0.0f));
}
void Surveyor::drawSurveyor(glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, const Model& surveyor, GLuint bakedTexture, GLint uTextureName, bool left, bool right)
{
    // Configuration des matrices de transformation et envoi au shader
    MVMatrix = glm::translate(MVMatrix, m_cam_position);
    MVMatrix = glm::translate(MVMatrix, glm::vec3(0, -2.15, -5.0f));
    MVMatrix = glm::scale(MVMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
    MVMatrix = glm::rotate(MVMatrix, m_theta / 3, glm::vec3(1.0f, 0.0f, 0.0f));
    MVMatrix = glm::rotate(MVMatrix, m_phi, glm::vec3(0.0f, 1.0f, 0.0f));
    if (left)
    {
        MVMatrix = glm::rotate(MVMatrix, glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (right)
    {
        MVMatrix = glm::rotate(MVMatrix, glm::radians(-50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniform1i(uTextureName, 0);
    // on bind les matrices au shader
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glBindTexture(GL_TEXTURE_2D, bakedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // on dessine notre surveyor
    surveyor.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}
void moveSurveyor(Surveyor& surveyor, bool& left, bool& right, bool& up, bool& down, p6::Context& ctx)
{
    // Gestion des commandes de la caméra par le joueur
    if (right)
    {
        surveyor.moveToLeft(-0.09f); // va à droite
    }
    if (left)
    {
        surveyor.moveToLeft(+0.09f); // va à gauche
    }
    if (up)
    {
        surveyor.moveToFront(0.09f); // avance
    }
    if (down)
    {
        surveyor.moveToFront(-0.09f); // recule
    }

    ctx.key_pressed = [&right, &up, &left, &down](p6::Key key) { // avancer avec les touches zqsd
        if (key.logical == "z")
        {
            up = true;
        }
        if (key.logical == "q")
        {
            left = true;
        }
        if (key.logical == "s")
        {
            down = true;
        }
        if (key.logical == "d")
        {
            right = true;
        }
    };

    ctx.key_released = [&right, &up, &left, &down](p6::Key key) {
        if (key.logical == "z")
        {
            up = false;
        }
        if (key.logical == "q")
        {
            left = false;
        }
        if (key.logical == "s")
        {
            down = false;
        }
        if (key.logical == "d")
        {
            right = false;
        }
    };
    // regarder à gauche à droite avec la souris comme dans Minecraft
    ctx.mouse_dragged = [&surveyor](const p6::MouseDrag& button) {
        surveyor.rotateToLeft(button.delta.x * 25);
        surveyor.rotateToUp(-button.delta.y * 25);
    };

    ctx.mouse_scrolled = [&](p6::MouseScroll scroll) {
        surveyor.moveToFront(-scroll.dy);
    };
}
