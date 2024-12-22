#include "GRectangleTexture.h"
#include <vector>
#include "GVector.h"
#include "GMatrix.h"
#include "GGraphicalComponent.h"
#include <functional> 

GGraphicalComponent::GGraphicalComponent() : textures(), texturePosRot() {
    this->controller = GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}};
}

GGraphicalComponent::GGraphicalComponent(std::vector<GRectangleTexture> textures, std::vector<GMatrix<4,4>> texturePosRot, GMatrix<4,4> controller) : 
                textures(textures), texturePosRot(texturePosRot), controller(controller) {}

void GGraphicalComponent::addTexturePiece(GRectangleTexture newTexture, GMatrix<4,4> newPosRot){
    textures.push_back(newTexture);
    texturePosRot.push_back(newPosRot);
}

void GGraphicalComponent::applyMovement(GMatrix<4,4> newMovement){
    controller *= newMovement;
}

void GGraphicalComponent::applyTranslation(GVector<3> newTranslation){
    controller[12] += newTranslation[0];
    controller[13] += newTranslation[1];
    controller[14] += newTranslation[2];
}

void GGraphicalComponent::applyRotation(GMatrix<3,3> newRotation){
    controller[0] += newRotation[0];
    controller[1] += newRotation[1];
    controller[2] += newRotation[2];
    controller[4] += newRotation[3];
    controller[5] += newRotation[4];
    controller[6] += newRotation[5];
    controller[8] += newRotation[6];
    controller[9] += newRotation[7];
    controller[10] += newRotation[8];
}

void GGraphicalComponent::setController(GMatrix<4,4> newController){
    controller = newController;
}

std::vector<GRectangleTexture> GGraphicalComponent::getTextures(){
    return textures;
}

std::vector<GMatrix<4,4>> GGraphicalComponent::getTextureTruePosRot(){
    //std::transform(texturePosRot.begin(), texturePosRot.end(), texturePosRot.begin(), std::bind(std::multiplies<GMatrix<4,4>>(), std::placeholders::_1, controller));
    // Above code does not work yet
    std::vector<GMatrix<4,4>> truePosRot = std::vector<GMatrix<4,4>>(texturePosRot.size());
    for (size_t i=0;i<texturePosRot.size();i++) {
        // Verify that this produces the correct result?
        truePosRot[i] = controller*texturePosRot[i];
    }
    return truePosRot;
}
