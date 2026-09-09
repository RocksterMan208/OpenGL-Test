#pragma once
#include <vector>
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include <glm/glm.hpp>

enum MeshType { CUBE };

class Mesh
{
public:
    Mesh(enum MeshType type);

    void Draw();

    std::vector<GLfloat> vertex;
    std::vector<GLuint> indices;

private:
    VAO vao;
    VBO* vbo = nullptr;
    EBO* ebo = nullptr;
};

std::vector<glm::vec3> genChunk(int chunkX, int chunkY, int chunkZ);