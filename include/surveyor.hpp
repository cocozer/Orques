#pragma once

#include <glm/glm.hpp>
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Surveyor {
private:
    glm::vec3 m_position; // camera position
    GLfloat   m_camera_horizontal;
    GLfloat   m_camera_vertical;
    GLfloat   m_camera_distance;
    bool      m_go_right;
    bool      m_go_left;
    bool      m_go_forward;
    bool      m_go_backward;

public:
    explicit Surveyor(glm::vec3 posSurveyor);
    explicit Surveyor(glm::vec3 posSurveyor, GLfloat camera_horizontal, GLfloat camera_vertical, GLfloat camera_distance);
    Surveyor();

    void      move(glm::vec3 distance);
    void      moveCameraSide(float distance);
    void      moveCameraForward(float distance);
    void      rotateCameraHorizontal(float angle);
    void      rotateCameraVertical(float angle);
    glm::mat4 updateCamera();
    void      cameraChange(bool& left, bool& right, bool& up, bool& down);
    void      setPosition(glm::vec3 posSurveyor);
    glm::vec3 getPosition();
    void      setCameraHorizontal(float angle);
    GLfloat   getCameraHorizontal() const;
    void      setCameraVertical(float angle);
    GLfloat   getCameraVertical() const;
    void      setDistance(GLfloat distance);
    GLfloat   getCameraDistance() const;
    void      draw(glm::mat4 ViewMatrix, std::vector<glimac::ShapeVertex> vertices, glm::mat4 ProjMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix);
};