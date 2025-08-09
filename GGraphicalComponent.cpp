#include "GRectangleTexture.h"
#include <vector>
#include "GVector.h"
#include "GMatrix.h"
#include "GGraphicalComponent.h"
#include <functional> 

GGraphicalComponent::GGraphicalComponent() : textures(), texturePosRot() {}

GGraphicalComponent::GGraphicalComponent(std::vector<GRectangleTexture> textures, std::vector<GMatrix<4,4>> texturePosRot) : 
                textures(textures), texturePosRot(texturePosRot) {}

void GGraphicalComponent::addTexturePiece(GRectangleTexture newTexture, GMatrix<4,4> newPosRot){
    textures.push_back(newTexture);
    texturePosRot.push_back(newPosRot);
}


std::vector<GRectangleTexture> GGraphicalComponent::getTextures(){
    return textures;
}

std::vector<GMatrix<4,4>> GGraphicalComponent::getTextureTruePosRot(GMatrix<4,4> originPoint){
    //std::transform(texturePosRot.begin(), texturePosRot.end(), texturePosRot.begin(), std::bind(std::multiplies<GMatrix<4,4>>(), std::placeholders::_1, originPoint));
    // Above code does not work yet
    
    std::vector<GMatrix<4,4>> truePosRot = std::vector<GMatrix<4,4>>(texturePosRot.size());
    for (size_t i=0;i<texturePosRot.size();i++) {
        truePosRot[i] = originPoint*texturePosRot[i];
    }
    return truePosRot;
}
