#include "GRectangleTexture.h"
#include <vector>
#include "GVector.h"
#include "GMatrix.h"


GRectangleTexture::GRectangleTexture(int srcHeight, int srcWidth, int textureID) : textureID(textureID) {
    this->srcHeight = srcHeight;
    this->srcWidth = srcWidth;
    this->subdivisionCount = 0;
    this->vertices = {GVector<2>{0,0},GVector<2>{0,srcHeight},GVector<2>{srcWidth,0},GVector<2>{srcWidth,srcHeight}};
}

GRectangleTexture::GRectangleTexture(int srcHeight, int srcWidth, int textureID, int subdivisionCount){
    GRectangleTexture newTexture = GRectangleTexture(srcHeight, srcWidth, textureID);
    newTexture.changeSubdivisionCount(subdivisionCount);
    *this = newTexture;
}

void GRectangleTexture::changeSubdivisionCount(int subdivisionCount){
    this->subdivisionCount = subdivisionCount;
    int columnCount = (2+subdivisionCount);
    int vertexCount = columnCount*columnCount;
    
    std::vector<GVector<2>> newVertices;
    newVertices.reserve(vertexCount);

    for (int c=0;c<columnCount;c++) {
        for (int r=0;r<columnCount;r++) {
            newVertices.push_back(GVector<2>{0+((this->srcWidth)*c/(columnCount-1)),0+((this->srcHeight)*r/(columnCount-1))});
        }
    }

    this->vertices = newVertices;

}

std::vector<GVector<2>> GRectangleTexture::getSrcVertices(){
    return this->vertices;
}

int GRectangleTexture::getTextureID(){
    return this->textureID;
}

