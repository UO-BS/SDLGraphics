#ifndef G_RECTANGLE_TEXTURE_H
#define G_RECTANGLE_TEXTURE_H

#include <vector>
#include "GVector.h"
#include "GMatrix.h"

// This class represents a 2D Texture (rectangular shape)

// A simple rectangle has 4 vertices (the corners), but this class was created for the situation in which we may need more vertices;
// Specifically, we approximate a Projective Transformation using Affine, which requires subdividing the rectangle into more vertices.
// The textureId is for holding an indentifier of what texture to use when displaying

class GRectangleTexture
{
public:

    //Vertices are in order of vertical first. So for 4x4 vertices, [4] is row 1 col 2

    // Source image details
    float srcHeight;
    float srcWidth;
    int subdivisionCount;
    int textureID;
    std::vector<GVector<2>> vertices;

    //constructors
    GRectangleTexture(int srcHeight, int srcWidth, int textureID);
    GRectangleTexture(int srcHeight, int srcWidth, int textureID, int subdivisionCount);

    //methods
    void changeSubdivisionCount(int subdivisionCount);
    std::vector<GVector<2>> getSrcVertices();
    int getTextureID();

};

#endif