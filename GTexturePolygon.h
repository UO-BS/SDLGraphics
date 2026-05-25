#ifndef G_TEXTURE_POLYGON_H
#define G_TEXTURE_POLYGON_H

#include <vector>
#include "GVector.h"
#include "GMatrix.h"
#include <memory>
#include "CommonStructs.h"

// This class represents a 2D Texture (polygonal shape)

class GTexturePolygon {
public:
    
    // The triangle vertices. 
    std::vector<GTextureTriangle> triangles;

    // The texture ID
    uint32_t textureID;

    // TODO: Polygon Constructor
    //GTexturePolygon(std::vector<GTextureVertex> vertices, uint32_t textureID);

    // Triangle Constructor
    GTexturePolygon(std::vector<GTextureTriangle> triangles, uint32_t textureID);
    // Rectangle Constructor
    GTexturePolygon(int newHeight, int newWidth, uint32_t textureID);
    // Copy Constructor
    GTexturePolygon(const GTexturePolygon& other);

    // TODO: Add helper methods for clipping, subdividing

    // Helper method to multiply each vertex by a transformation matrix
    GTexturePolygon& applyTransformation(GMatrix<4,4> transformMatrix);

private:

};

#endif