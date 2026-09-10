#include "mesh.h"
#include <unordered_set>
#include <functional>

const GLfloat vertexListCube[] = {
    // positions          // normals           // texcoords
    // Front face (+Z)
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

    // Back face (-Z)
    0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

    // Left face (-X)
    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

    // Right face (+X)
    0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

    // Top face (+Y)
    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

    // Bottom face (-Y)
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
};

constexpr GLuint indicesListCube[] = {
    0,  1,  2,   2,  3,  0,   // front
    4,  5,  6,   6,  7,  4,   // back
    8,  9,  10,  10, 11, 8,   // left
    12, 13, 14,  14, 15, 12,  // right
    16, 17, 18,  18, 19, 16,  // top
    20, 21, 22,  22, 23, 20,  // bottom
};

// Same order as the face blocks in vertexListCube above: front, back, left, right, top, bottom
static const glm::ivec3 faceNormals[6] = {
    {0, 0, 1}, {0, 0, -1}, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0}
};

struct IVec3Hash
{
    size_t operator()(const glm::ivec3& v) const
    {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1) ^ (std::hash<int>()(v.z) << 2);
    }
};

Mesh::Mesh(enum MeshType type)
{
    if (type == CUBE)
    {
        for (size_t i = 0; i < sizeof(vertexListCube) / sizeof(vertexListCube[0]); i++)
            vertex.push_back(vertexListCube[i]);

        for (size_t i = 0; i < sizeof(indicesListCube) / sizeof(indicesListCube[0]); i++)
            indices.push_back(indicesListCube[i]);
    }

    setupMesh();
}

Mesh::Mesh(const std::vector<GLfloat>& verts, const std::vector<GLuint>& inds)
{
    vertex = verts;
    indices = inds;

    setupMesh();
}

void Mesh::setupMesh()
{
    vao.Bind();

    vbo = new VBO(vertex.data(), vertex.size() * sizeof(GLfloat));
    ebo = new EBO((GLint*)indices.data(), indices.size() * sizeof(GLuint));

    // position (location 0)
    vao.LinkAttrib(*vbo, 0, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)0);
    // normal (location 1)
    vao.LinkAttrib(*vbo, 1, 3, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    // texcoord (location 2)
    vao.LinkAttrib(*vbo, 2, 2, GL_FLOAT, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    vao.Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

void Mesh::Draw()
{
    vao.Bind();
    ebo->Bind();
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}

std::vector<glm::vec3> genChunk(int chunkX, int chunkY, int chunkZ)
{
    std::vector<glm::vec3> output;

    for (float x = 0.0f; x < chunkX; x++)
    {
        for (float y = 0.0f; y < chunkY; y++)
        {
            for (float z = 0.0f; z < chunkZ; z++)
            {
                output.push_back(glm::vec3(x, y, z));
            }
        }
    }

    return output;
}

Mesh buildChunkMesh(int chunkX, int chunkY, int chunkZ)
{
    std::vector<glm::vec3> blockPositions = genChunk(chunkX, chunkY, chunkZ);

    std::unordered_set<glm::ivec3, IVec3Hash> blocks;
    for (const auto& p : blockPositions) blocks.insert(glm::ivec3((int)p.x, (int)p.y, (int)p.z));

    std::vector<GLfloat> verts;
    std::vector<GLuint> inds;

    for (const auto& p : blockPositions)
    {
        glm::ivec3 b((int)p.x, (int)p.y, (int)p.z);

        for (int face = 0; face < 6; face++)
        {
            glm::ivec3 neighbor = b + faceNormals[face];

            if (blocks.count(neighbor) > 0)
                continue;

            GLuint base = (GLuint)(verts.size() / 8);

            for (int v = 0; v < 4; v++)
            {
                int srcIndex = (face * 4 + v) * 8;

                verts.push_back(vertexListCube[srcIndex + 0] + p.x);
                verts.push_back(vertexListCube[srcIndex + 1] + p.y);
                verts.push_back(vertexListCube[srcIndex + 2] + p.z);
                verts.push_back(vertexListCube[srcIndex + 3]);
                verts.push_back(vertexListCube[srcIndex + 4]);
                verts.push_back(vertexListCube[srcIndex + 5]);
                verts.push_back(vertexListCube[srcIndex + 6]);
                verts.push_back(vertexListCube[srcIndex + 7]);
            }

            inds.push_back(base + 0);
            inds.push_back(base + 1);
            inds.push_back(base + 2);
            inds.push_back(base + 2);
            inds.push_back(base + 3);
            inds.push_back(base + 0);
        }
    }

    return Mesh(verts, inds);
}
