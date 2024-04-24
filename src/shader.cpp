#include "shader.hpp"
#include <p6/p6.h>
#include <string>
#include <utility>
#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

void Shader::inituMVPMatrix(std::string uName)
{
    uMVPMatrix = glGetUniformLocation(m_shader.id(), uName.c_str());
}
void Shader::inituMVMatrix(std::string uName)
{
    uMVMatrix = glGetUniformLocation(m_shader.id(), uName.c_str());
}
void Shader::inituNormalMatrix(std::string uName)
{
    uNormalMatrix = glGetUniformLocation(m_shader.id(), uName.c_str());
}
void Shader::use()
{
    m_shader.use();
}