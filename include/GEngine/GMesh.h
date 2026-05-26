#ifndef G_MESH_H
#define G_MESH_H

#include <glad/glad.h>
#include <vector>
#include "GMatrix.h"
#include "GVector.h"
#include "GCommonStructs.h"

/*
    Represents a Mesh loaded into the GPU
    The actual vertices are not stored here
*/ 

class GMesh
{
public:

    // OpenGL Handles
    uint32_t VAO = 0;
    uint32_t VBO = 0;

    // Since the vertices aren't stored in this object anymore, we cant use .size(); so keep track of the size.
    GLsizei totalVertexCount = 0;

    const GTexture* texture = nullptr;

    GMesh(const std::vector<GTextureVertex>& vertices, const GTexture* texture);

    ~GMesh();

    // No copying
    GMesh(const GMesh&) = delete;
    GMesh& operator=(const GMesh&) = delete;

    void draw() const;

};

#endif