#ifndef G_GRAPHICAL_COMPONENT_H
#define G_GRAPHICAL_COMPONENT_H

#include <vector>
#include "GMatrix.h"
#include "GVector.h"
#include "GRectangleTexture.h"

// This class represents the GraphicalComponent of an object in 3D space

// The GraphicalComponent holds the Position and Rotation of all textures associated with the object
// It is composed of many GRectangleTextures 'textures', which each have a relative position/rotation 'texturePosRot'
// For example; a cube has 6 'textures' and the 'texturePosRot' is what arranges the 6 sides into the cube shape

class GGraphicalComponent
{
public:

    // Use this convention: https://stackoverflow.com/questions/10094634/4x4-matrix-last-element-significance 

    // These textures form the graphical "object"
    std::vector<GRectangleTexture> textures;
    std::vector<GMatrix<4,4>> texturePosRot;

    //constructors
    GGraphicalComponent();
    GGraphicalComponent(std::vector<GRectangleTexture> textures, std::vector<GMatrix<4,4>> texturePosRot);

    //Controlling individual textures
    void addTexturePiece(GRectangleTexture newTexture, GMatrix<4,4> newPosRot);

    // Get texture details
    std::vector<GRectangleTexture> getTextures();
    // Get the coordinates of the textures based on the position/rotation of a given origin point.
    std::vector<GMatrix<4,4>> getTextureTruePosRot(GMatrix<4,4> originPoint);

};

#endif