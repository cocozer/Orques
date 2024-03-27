#include "surveyor.hpp"
#include <p6/p6.h>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

Surveyor::Surveyor()
    : m_position(0., 0., 0.), m_camera_horizontal(0.0f), m_camera_vertical(0.0f), m_camera_distance(0.5f){};

Surveyor::Surveyor(glm::vec3 posSurveyor)
    : m_position(posSurveyor), m_camera_horizontal(0.0f), m_camera_vertical(0.0f), m_camera_distance(0.5f){};

Surveyor::Surveyor(glm::vec3 posSurveyor, GLfloat camera_horizontal, GLfloat camera_vertical, GLfloat camera_distance)
    : m_position(posSurveyor), m_camera_horizontal(camera_horizontal), m_camera_vertical(camera_vertical), m_camera_distance(camera_distance){};

void Surveyor::setPosition(glm::vec3 posSurveyor)
{
    m_position = posSurveyor;
}

glm::vec3 Surveyor::getPosition()
{
    return m_position;
}

void Surveyor::setDistance(GLfloat distance)
{
    m_camera_distance = distance;
}

GLfloat Surveyor::getCameraDistance() const
{
    return m_camera_distance;
}

void Surveyor::setCameraHorizontal(float angle)
{
    m_camera_horizontal = angle;
}

GLfloat Surveyor::getCameraHorizontal() const
{
    return m_camera_horizontal;
}

void Surveyor::setCameraVertical(float angle)
{
    m_camera_vertical = angle;
}

GLfloat Surveyor::getCameraVertical() const
{
    return m_camera_vertical;
}

void Surveyor::move(glm::vec3 distance)
{
    m_position += distance;
}

void Surveyor::moveCameraSide(float distance)
{
    glm::vec3 position = {distance * sin((m_camera_horizontal + 90.0f) * p6::PI / 180.0f), 0.0f, -distance * cos((m_camera_horizontal + 90.0f) * p6::PI / 180.0f)};
    move(position);
}

void Surveyor::moveCameraForward(float distance)
{
    glm::vec3 position = {distance * sin(m_camera_horizontal * p6::PI / 180.0f), 0.0f, -distance * cos(m_camera_horizontal * p6::PI / 180.0f)};
    move(position);
}

void Surveyor::rotateCameraHorizontal(float angle)
{
    m_camera_horizontal += angle;
}
void Surveyor::rotateCameraVertical(float angle)
{
    m_camera_vertical += glm::radians(angle);
    if (m_camera_vertical > 1.5f)
    {
        m_camera_vertical = 1.5f;
    }
}
glm::mat4 Surveyor::updateCamera()
{
    glm::vec3 position;
    position.x = getPosition().x + m_camera_distance * sin(m_camera_horizontal * p6::PI / 180.0f) * cos(m_camera_vertical * p6::PI / 180.0f);
    position.y = getPosition().y + m_camera_distance * sin(m_camera_vertical * p6::PI / 180.0f);
    position.z = getPosition().z + m_camera_distance * cos(m_camera_horizontal * p6::PI / 180.0f) * cos(m_camera_vertical * p6::PI / 180.0f);

    return glm::lookAt(position, getPosition(), glm::vec3(0.0f, 1.0f, 0.f));
}

void Surveyor::draw(glm::mat4 ViewMatrix, std::vector<glimac::ShapeVertex> vertices, glm::mat4 ProjMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix)
{
    ViewMatrix = glm::translate(ViewMatrix, m_position);
    ViewMatrix = glm::translate(ViewMatrix, glm::vec3(0, -0.5, 0.0));

    ViewMatrix             = glm::scale(ViewMatrix, glm::vec3(0.2, 0.2, 0.2));
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrix));

    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * ViewMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
}

void moveSurveyor(Surveyor& surveyor, bool& left, bool& right, bool& up, bool& down, p6::Context& ctx)
{
    if (right)
    {
        surveyor.moveCameraSide(-0.02f);
        surveyor.move(glm::vec3(0.02f, 0.0f, 0.0f));
    }
    if (left)
    {
        surveyor.moveCameraSide(0.02f);
        surveyor.move(glm::vec3(-0.02f, 0.0f, 0.0f));
    }
    if (up)
    {
        surveyor.moveCameraForward(0.02f);
        surveyor.move(glm::vec3(0.00f, 0.0f, 0.02f));
    }
    if (down)
    {
        surveyor.moveCameraForward(-0.02f);
        surveyor.move(glm::vec3(0.00f, 0.0f, -0.02f));
    }

    ctx.key_pressed = [&right, &up, &left, &down](const p6::Key& key) {
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

    ctx.key_released = [&right, &up, &left, &down](const p6::Key& key) {
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

    ctx.mouse_dragged = [&surveyor](const p6::MouseDrag& button) {
        surveyor.rotateCameraHorizontal(button.delta.x * 5);
        surveyor.rotateCameraVertical(-button.delta.y * 5);
    };
}