#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include<atomic>
#include<thread>

#include "FastNoiseLite.h"

extern float intensity;

enum blockType : uint8_t
{
    voxel_air,
    voxel_grass,
    voxel_dirt,
    voxel_stone
};

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 norm;
    glm::vec2 uv;
};

struct ChunkMesh
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

class Chunk
{
public:
    std::vector<blockType> blocks;

    int getIndex(glm::ivec3 pos) const;

    Chunk(
        int sizeX,
        int sizeY,
        int sizeZ,
        glm::ivec3 offset = glm::ivec3(0)
    );

    void generate();

    void setBlock(glm::ivec3 pos, blockType block);

    ChunkMesh genMesh() const;

    bool isSolid(glm::ivec3 pos) const;

    void addFace(
        ChunkMesh& mesh,
        const glm::ivec3& blockPos,
        int face
    ) const;

private:
    int sizeX;
    int sizeY;
    int sizeZ;

    glm::ivec3 offset;

    FastNoiseLite noise;

    blockType getBlock(glm::ivec3 pos) const;
};

class ChunkRenderMesh
{
public:
    ChunkRenderMesh();
    ~ChunkRenderMesh();

    void upload(const ChunkMesh& mesh);
    void draw();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLsizei indexCount;
};

class World
{
    public:
    std::vector<Chunk> chunks;
    std::vector<ChunkRenderMesh> renderMeshes;

    World(
        glm::vec3 chunkSize = glm::vec3(16.0f, 32.0f, 16.0f),
        glm::ivec2 num = glm::ivec2(1)
    );

    ~World();

    void generate();
    void buildMeshes();
    void draw();

    private:

    std::thread worker;
    std::atomic<bool> generating = false;
};