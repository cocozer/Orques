#pragma once

#include <filesystem>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Shader {
private:
    p6::Shader m_shader;

    glm::mat4 ProjMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;

public:
    Shader(std::filesystem::path vsPath, std::filesystem::path fsPath)
        : m_shader(p6::load_shader(vsPath, fsPath)){};
    void inituMVPMatrix(std::string uName);
    void inituMVMatrix(std::string uName);
    void inituNormalMatrix(std::string uName);

    glm::mat4 getProjMatrix() const { return ProjMatrix; }
    glm::mat4 getMVMatrix() const { return MVMatrix; }
    glm::mat4 getNormalMatrix() const { return NormalMatrix; }
    GLint     getuMVPMatrix() const { return uMVPMatrix; }
    GLint     getuMVMatrix() const { return uMVMatrix; }
    GLint     getuNormalMatrix() const { return uNormalMatrix; }

    glm::mat4 setProjMatrix(glm::mat4 matrix) { ProjMatrix = matrix; }
    glm::mat4 setMVMatrix(glm::mat4 matrix) { MVMatrix = matrix; }
    glm::mat4 setNormalMatrix(glm::mat4 matrix) { NormalMatrix = matrix; }
    GLint     setuMVPMatrix(GLint u) { uMVPMatrix = u; }
    GLint     setuMVMatrix(GLint u) { uMVMatrix = u; }
    GLint     setuNormalMatrix(GLint u) { uNormalMatrix = u; }

    void use();
};
