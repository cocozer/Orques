#include "light.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

void Light::drawLightScene(glm::vec3 position, glm::mat4 projMatrix, glm::mat4 MVMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix, const p6::Shader& shader, GLint uKd, GLint uKs, GLint uShininess, GLint uLightPos_vs, GLint uLightIntensity)
{
    shader.use();
    glUniform3fv(uKd, 1, glm::value_ptr(glm::vec3(2.7f, 2.7f, 2.8f))); // réflexion diffuse
    glUniform3fv(uKs, 1, glm::value_ptr(glm::vec3(1.7f, 1.7f, 1.9f))); // réflexion spéculaire
    glUniform1f(uShininess, 0.11);

    const float radiusChangeAmount = 5.f;
    glm::vec3   newPosition        = position + glm::vec3(1.0f, 0.0f, 0.0f) * radiusChangeAmount;

    glm::vec4 LightPos = MVMatrix * glm::vec4(newPosition, 1.0);

    glUniform3fv(uLightPos_vs, 1, glm::value_ptr(glm::vec3(LightPos.x, LightPos.y, LightPos.z)));
    glUniform3fv(uLightIntensity, 1, glm::value_ptr(this->m_light_intensity));

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
}

void Light::drawLightSurveyor(glm::vec3 position, glm::mat4 projMatrix, glm::mat4 MVMatrix, GLint uMVPMatrix, GLint uMVMatrix, GLint uNormalMatrix, const p6::Shader& shader, GLint uKd, GLint uKs, GLint uShininess2, GLint uLightPos2_vs, GLint uLightIntensity2)
{
    shader.use();
    // glUniform3fv(uKd, 1, glm::value_ptr(glm::vec3(1.7f, 1.7f, 1.8f))); // réflexion diffuse
    // glUniform3fv(uKs, 1, glm::value_ptr(glm::vec3(0.7f, 0.7f, 1.7f))); // réflexion spéculaire
    glUniform1f(uShininess2, 0.05);
    glm::vec4 LightPos = MVMatrix * glm::vec4(position, 1.0);
    glUniform3fv(uLightPos2_vs, 1, glm::value_ptr(glm::vec3(LightPos.x, LightPos.y, LightPos.z)));
    glUniform3fv(uLightIntensity2, 1, glm::value_ptr(this->m_light_intensity));

    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(projMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
}
