#include "chunk.h"

const GLfloat vertexListCube[] =
{
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

     0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

     0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,  0.0f, 1.0f
};

constexpr GLuint indicesListCube[] =
{
     0,  1,  2,  2,  3,  0,
     4,  5,  6,  6,  7,  4,
     8,  9, 10, 10, 11,  8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
};

static const glm::ivec3 faceNormals[6] =
{
    { 0,  0,  1 },
    { 0,  0, -1 },
    {-1,  0,  0 },
    { 1,  0,  0 },
    { 0,  1,  0 },
    { 0, -1,  0 }
};

Chunk::Chunk(
    int sizeX,
    int sizeY,
    int sizeZ,
    glm::ivec3 offset
)
    : sizeX(sizeX),
      sizeY(sizeY),
      sizeZ(sizeZ),
      offset(offset),
      blocks(sizeX * sizeY * sizeZ, voxel_air)
{
    noise.SetSeed(12345);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noise.SetFractalOctaves(5);
    noise.SetFractalLacunarity(2.0f);
    noise.SetFractalGain(0.5f);
    noise.SetFrequency(0.005f);
}

int Chunk::getIndex(glm::ivec3 pos) const
{
    return pos.x + sizeX * (pos.z + sizeZ * pos.y);
}

blockType Chunk::getBlock(glm::ivec3 pos) const
{
    return blocks[getIndex(pos)];
}

void Chunk::setBlock(glm::ivec3 pos, blockType block)
{
    blocks[getIndex(pos)] = block;
}

void Chunk::generate()
{
    for (int x = 0; x < sizeX; x++)
    {
        for (int z = 0; z < sizeZ; z++)
        {
            int worldX = offset.x + x;
            int worldZ = offset.z + z;

            float n = noise.GetNoise(
                static_cast<float>(worldX),
                static_cast<float>(worldZ)
            );

            n = (n + 1.0f) * 0.5f;

            int height = static_cast<int>(n * static_cast<float>(sizeY - 1) * intensity);

            for (int y = 0; y < sizeY; y++)
            {
                if (y > height)
                {
                    setBlock(
                        glm::ivec3(x, y, z),
                        voxel_air
                    );
                }
                else if (y == height)
                {
                    setBlock(
                        glm::ivec3(x, y, z),
                        voxel_grass
                    );
                }
                else if (y > height - 3)
                {
                    setBlock(
                        glm::ivec3(x, y, z),
                        voxel_dirt
                    );
                }
                else
                {
                    setBlock(
                        glm::ivec3(x, y, z),
                        voxel_stone
                    );
                }
            }
        }
    }
}

bool Chunk::isSolid(glm::ivec3 pos) const
{
    if (
        pos.x < 0 || pos.x >= sizeX ||
        pos.y < 0 || pos.y >= sizeY ||
        pos.z < 0 || pos.z >= sizeZ
    )
    {
        return false;
    }

    return getBlock(pos) != voxel_air;
}

void Chunk::addFace(
    ChunkMesh& mesh,
    const glm::ivec3& blockPos,
    int face
) const
{
    GLuint base = static_cast<GLuint>(mesh.vertices.size());

    for (int i = 0; i < 4; i++)
    {
        int index = (face * 4 + i) * 8;

        Vertex vertex;

        vertex.pos =
        {
            vertexListCube[index + 0] + blockPos.x + offset.x,
            vertexListCube[index + 1] + blockPos.y + offset.y,
            vertexListCube[index + 2] + blockPos.z + offset.z
        };

        vertex.norm =
        {
            vertexListCube[index + 3],
            vertexListCube[index + 4],
            vertexListCube[index + 5]
        };

        vertex.uv =
        {
            vertexListCube[index + 6],
            vertexListCube[index + 7]
        };

        mesh.vertices.push_back(vertex);
    }

    mesh.indices.push_back(base + 0);
    mesh.indices.push_back(base + 1);
    mesh.indices.push_back(base + 2);
    mesh.indices.push_back(base + 2);
    mesh.indices.push_back(base + 3);
    mesh.indices.push_back(base + 0);
}

ChunkMesh Chunk::genMesh() const
{
    ChunkMesh data;

    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            for (int z = 0; z < sizeZ; z++)
            {
                glm::ivec3 pos(x, y, z);

                if (!isSolid(pos))
                    continue;

                for (int face = 0; face < 6; face++)
                {
                    glm::ivec3 neighbour =
                        pos + faceNormals[face];

                    if (!isSolid(neighbour))
                    {
                        addFace(data, pos, face);
                    }
                }
            }
        }
    }

    return data;
}

ChunkRenderMesh::ChunkRenderMesh()
    : VAO(0),
      VBO(0),
      EBO(0),
      indexCount(0)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

ChunkRenderMesh::~ChunkRenderMesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void ChunkRenderMesh::upload(const ChunkMesh& mesh)
{
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh.vertices.size() * sizeof(Vertex),
        mesh.vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh.indices.size() * sizeof(uint32_t),
        mesh.indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    indexCount =
        static_cast<GLsizei>(mesh.indices.size());
}

void ChunkRenderMesh::draw()
{
    glBindVertexArray(VAO);

    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        nullptr
    );

    glBindVertexArray(0);
}

World::World(glm::vec3 chunkSize, glm::ivec2 num)
{
    for (int x = 0; x < num.x; x++)
    {
        for (int z = 0; z < num.y; z++)
        {
            chunks.emplace_back(
                static_cast<int>(chunkSize.x),
                static_cast<int>(chunkSize.y),
                static_cast<int>(chunkSize.z),
                glm::ivec3(
                    x * static_cast<int>(chunkSize.x),
                    0,
                    z * static_cast<int>(chunkSize.z)
                )
            );
        }
    }

    renderMeshes.resize(chunks.size());
}

World::~World()
{
    if(worker.joinable())
    {
        worker.join();
    }
}

void World::generate()
{
    worker = std::thread([this]()
    {
        for (Chunk& chunk : chunks)
        {
            chunk.generate();
        }
    });
}

void World::buildMeshes()
{
    if (worker.joinable())
        worker.join();

    for (size_t i = 0; i < chunks.size(); i++)
    {
        ChunkMesh mesh = chunks[i].genMesh();
        renderMeshes[i].upload(mesh);
    }
}

void World::draw()
{
    for (ChunkRenderMesh& mesh : renderMeshes)
    {
        mesh.draw();
    }
}