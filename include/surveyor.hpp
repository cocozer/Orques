#pragma once

#include <glm/glm.hpp>
#include "glimac/sphere_vertices.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model.hpp"
#include "p6/p6.h"

class Surveyor {
private:
    Model&    m_model;
    float     m_cam_distance;
    float     m_cam_height;
    glm::vec3 m_cam_position;
    float     m_phi;
    float     m_theta;
    float     m_rotation_angle;

    glm::vec3 m_front_vector;
    glm::vec3 m_left_vector;
    glm::vec3 m_up_vector;

    void setDirectionVectors();

public:
    explicit Surveyor(Model& model);
    void update_camera(glm::mat4& viewMatrix);
    ~Surveyor() = default;
    void      drawSurveyor(glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, const Model& surveyor, GLuint bakedTexture, GLint uTextureName);
    void      setPosition(glm::vec3 position_surveyor);
    glm::vec3 getPosition();
    void      setRotationAngle(float rotation_angle);
    float     getRotationAngle() const;
    void      moveToLeft(float t);
    void      moveToFront(float t);
    void      rotateToLeft(float degrees);
    void      rotateToUp(float degrees);
    glm::mat4 getViewMatrix() const;
};
void moveSurveyor(Surveyor& surveyor, bool& left, bool& right, bool& up, bool& down, p6::Context& ctx);