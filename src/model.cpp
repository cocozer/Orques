#include "model.hpp"
#include <glimac/common.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/matrix.hpp"
#include "random.hpp"
// utiliser la librairie tinyobj
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

// on initialise le vbo
void Model::setVbo()
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // bind du vbo
    glBufferData(GL_ARRAY_BUFFER, getVertexCount() * sizeof(glimac::ShapeVertex), getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // debind le vbo
}

// on initialise le vao
void Model::setVao()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao); // bind du vao

    glEnableVertexAttribArray(0); // Position
    glEnableVertexAttribArray(1); // Normale
    glEnableVertexAttribArray(2); // Coordonnées de texture

    // bind du vbo
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind le VBO
    glBindVertexArray(0);             // Debind le VAO
}

// pointeur vers les donnees des vertices
const glimac::ShapeVertex* Model::getDataPointer() const
{
    return m_vertices.data();
}

// load un modele a partir dun fichier blender .obj
void Model::loadModel(const std::string& fileName)
{
    std::string                      inputfile = "../assets/models/" + fileName;
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;

    // Charge le fichier .obj
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), nullptr);

    // gestion des erreurs
    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }

    if (!ret)
    {
        exit(1);
    }

    // Traitement des shapes du fichier
    for (const auto& shape : shapes)
    {
        size_t index_offset = 0;
        // Traitement des triangles du mesh
        for (int f = 0; f < shape.mesh.num_face_vertices.size(); f++)
        {
            int fv = shape.mesh.num_face_vertices[f];
            // Traitement des vertices de la face
            for (int v = 0; v < fv; v++)
            {
                tinyobj::index_t    idx    = shape.mesh.indices[index_offset + v];
                glimac::ShapeVertex vertex = {
                    glm::vec3(attrib.vertices[3 * idx.vertex_index + 0], attrib.vertices[3 * idx.vertex_index + 1], attrib.vertices[3 * idx.vertex_index + 2]),
                    glm::vec3(attrib.normals[3 * idx.normal_index + 0], attrib.normals[3 * idx.normal_index + 1], attrib.normals[3 * idx.normal_index + 2]),
                    glm::vec2(attrib.texcoords[2 * idx.texcoord_index + 0], attrib.texcoords[2 * idx.texcoord_index + 1])
                };
                m_vertices.push_back(vertex);
            }
            index_offset += fv;
        }
    }
    m_vertexCount = m_vertices.size(); // Mise à jour du compteur de vertices
}
void Model::drawModel(glm::vec3 position, glm::vec3 scale, glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, GLuint bakedTexture, GLint uTextureName)
{
    glm::mat4 ViewMatrixModel = glm::translate(glm::mat4(1.0), position);
    ViewMatrixModel           = glm::scale(ViewMatrixModel, scale);
    MVMatrix                  = MVMatrix * ViewMatrixModel;

    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniform1i(uTextureName, 0);
    // on bind les matrices au shader
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glBindTexture(GL_TEXTURE_2D, bakedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // on dessine notre cube
    draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Model::drawModel(glm::vec3 position, float angle, glm::vec3 scale, glm::mat4 MVMatrix, GLint uMVMatrix, GLint uMVPMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, GLint uNormalMatrix, GLuint bakedTexture, GLint uTextureName)
{
    glm::mat4 ViewMatrixModel = glm::translate(glm::mat4(1.0), position);
    ViewMatrixModel           = glm::scale(ViewMatrixModel, scale);
    ViewMatrixModel           = glm::rotate(ViewMatrixModel, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    MVMatrix = MVMatrix * ViewMatrixModel;

    NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

    glUniform1i(uTextureName, 0);
    // on bind les matrices au shader
    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
    glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glBindTexture(GL_TEXTURE_2D, bakedTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // on dessine notre cube
    draw();
    glBindTexture(GL_TEXTURE_2D, 0);
}