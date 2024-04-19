#pragma once

#include <glimac/common.hpp>
#include <string>
#include "glm/fwd.hpp"
#include "p6/p6.h"


class Texture {
private:
    GLuint                      m_id;
    std::string                 m_path;
    std::unique_ptr<img::Image> m_image;

public:
    Texture();

    Texture(const std::string& file);

    Texture(const Texture& texture);

    ~Texture()
    {
        glDeleteTextures(1, &m_id);
    }

    GLuint get_id() const;

    void loadTexture();
};