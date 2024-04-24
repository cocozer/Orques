#pragma once

#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Light {
private:
    glm::vec3 m_light_intensity;

public:
    Light(glm::vec3 intensity)
        : m_light_intensity(intensity){};
    ~Light() = default;

    void drawLightScene(glm::vec3 position, glm::mat4 projMatrix, glm::mat4 MVMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix, const p6::Shader& shader, GLint uKd, GLint uKs, GLint uShininess, GLint uLightPos_vs, GLint uLightIntensity);
    void drawLightSurveyor(glm::vec3 position, glm::mat4 projMatrix, glm::mat4 MVMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix, GLint uLightPos2_vs, GLint uLightIntensity2);
};