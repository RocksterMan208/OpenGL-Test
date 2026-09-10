#pragma once
#include <vector>
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include <glm/glm.hpp>

extern const GLfloat vertexListCube[];

enum MeshType { CUBE };

class Mesh
{
public:
    Mesh(enum MeshType type);
    Mesh(const std::vector<GLfloat>& verts, const std::vector<GLuint>& inds);

    void Draw();

    std::vector<GLfloat> vertex;
    std::vector<GLuint> indices;

private:
    void setupMesh();

    VAO vao;
    VBO* vbo = nullptr;
    EBO* ebo = nullptr;
};

std::vector<glm::vec3> genChunk(int chunkX, int chunkY, int chunkZ);
Mesh buildChunkMesh(int chunkX, int chunkY, int chunkZ);