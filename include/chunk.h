#ifndef CHUNK_CLASS_H
#define CHUNK_CLASS_H

#include<unordered_set>
#include<glm/glm.hpp>
#include"mesh.h"

enum blockType {GRASS_BLOCK, DIRT_BLOCK};

struct Ivec3Hash
{
    size_t operator()(const glm::ivec3& v) const 
    {
        return std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1) ^ (std::hash<int>()(v.z) << 2);
    }
};

class Chunk
{
    public:
        Chunk(int sizeX, int sizeY, int sizeZ, glm::ivec3 offset = glm::ivec3(0.0f));

        void setBlock(glm::ivec3 pos, bool solid);
        bool isSolid(glm::ivec3 pos);

        void rebuild();
        void draw();

    private:
        int sizeX, sizeY, sizeZ;
        glm::ivec3 offset = glm::ivec3(0.0f);
        std::unordered_set<glm::ivec3, Ivec3Hash> blocks;
        Mesh* mesh = nullptr;
        void removeBlock(glm::ivec3 pos);
        
        bool reset = false;
};

#endif