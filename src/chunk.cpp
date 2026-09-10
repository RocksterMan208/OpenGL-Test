#include"chunk.h"
static const glm::ivec3 faceNormals[6] = {
    {0, 0, 1}, {0, 0, -1}, {-1, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, -1, 0}
};

extern const GLfloat vertexListCube[];

Chunk::Chunk(int sizeX, int sizeY, int sizeZ, glm::ivec3 worldOffset) : sizeX(sizeX), sizeY(sizeY), sizeZ(sizeZ), offset(worldOffset)
{
    for (int x = 0; x < sizeX; x++)
    {
        for (int y = 0; y < sizeY; y++)
        {
            for (int z = 0; z < sizeZ; z++)
            {
                blocks.insert(glm::ivec3(x, y, z) + offset);
            }
        }
    }

    rebuild();
}

void Chunk::setBlock(glm::ivec3 pos, bool solid)
{
    if (solid)
    {
        blocks.insert(pos);
    }
    else
    {
        blocks.erase(pos);
    }
    reset = true;
}

void Chunk::removeBlock(glm::ivec3 pos)
{
    setBlock(pos, false);
}

void Chunk::rebuild()
{
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    for (const auto& b : blocks)
    {
        for ( int faces = 0; faces < 6; faces++)
        {
            glm::ivec3 neighbour = b + faceNormals[faces];

            if (blocks.count(neighbour) > 0) continue;

            GLuint base = static_cast<GLuint>(vertices.size()/8);

            for (int v = 0; v < 4; v++)
            {
                int srcIndex = (faces * 4 + v) * 8;

                vertices.push_back(vertexListCube[srcIndex + 0] + b.x);
                vertices.push_back(vertexListCube[srcIndex + 1] + b.y);
                vertices.push_back(vertexListCube[srcIndex + 2] + b.z);
                vertices.push_back(vertexListCube[srcIndex + 3]);
                vertices.push_back(vertexListCube[srcIndex + 4]);
                vertices.push_back(vertexListCube[srcIndex + 5]);
                vertices.push_back(vertexListCube[srcIndex + 6]);
                vertices.push_back(vertexListCube[srcIndex + 7]);
            }

            indices.push_back(base + 0);
            indices.push_back(base + 1);
            indices.push_back(base + 2);
            indices.push_back(base + 2);
            indices.push_back(base + 3);
            indices.push_back(base + 0);
        }
    }

    delete mesh;
    mesh = new Mesh(vertices, indices);
    reset = false;
}

void Chunk::draw()
{
    if (reset)
    {
        rebuild();
    }

    mesh->Draw();
}