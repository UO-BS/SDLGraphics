#ifndef G_GRAPHICAL_COMPONENT_H
#define G_GRAPHICAL_COMPONENT_H

#include <vector>
#include "GMatrix.h"
#include "GVector.h"
#include "GRectangleTexture.h"

class GGraphicalComponent
{
public:

    // Use this convention: https://stackoverflow.com/questions/10094634/4x4-matrix-last-element-significance 

    // These textures form the graphical "object"
    std::vector<GRectangleTexture> textures;
    std::vector<GMatrix<4,4>> texturePosRot;

    // This point controls the texture group (rotating this will rotate every texture)
    GMatrix<4,4> controller;

    //constructors
    GGraphicalComponent();
    GGraphicalComponent(std::vector<GRectangleTexture> textures, std::vector<GMatrix<4,4>> texturePosRot, GMatrix<4,4> controller);

    //Controlling individual textures
    void addTexturePiece(GRectangleTexture newTexture, GMatrix<4,4> newPosRot);

    // Controlling entire object
    void applyMovement(GMatrix<4,4> newMovement);
    void applyTranslation(GVector<3> newTranslation);
    void applyRotation(GMatrix<3,3> newRotation);
    void setController(GMatrix<4,4> newController);

    // Get texture details
    std::vector<GRectangleTexture> getTextures();
    // Get the true coordinates of the textures. This applies the controller Matrix to the individual texture matrices
    std::vector<GMatrix<4,4>> getTextureTruePosRot();

};

#endif