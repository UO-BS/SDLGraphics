#ifndef G_RECTANGLE_TEXTURE_H
#define G_RECTANGLE_TEXTURE_H

#include <vector>
#include "GVector.h"
#include "GMatrix.h"

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