#include "texture.hpp"

Texture::Texture()
    : m_id(0), m_path("")
{
}

Texture::Texture(const std::string& file)
    : m_id(0), m_path(file)
{
    loadTexture();
}

Texture::Texture(const Texture& texture)
    : m_id(0), m_path(texture.m_path)
{
    loadTexture();
}

GLuint Texture::get_id() const
{
    return m_id;
}

void Texture::loadTexture()
{
    *m_image = p6::load_image_buffer(m_path);

    if (m_image == nullptr)
    {
        std::cout << "Error, texture could not be loaded" << std::endl; // TO DO : exception
    }

    glGenTextures(1, &m_id);

    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image->width(), m_image->height(), 0, GL_RGBA, GL_FLOAT, m_image->data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}