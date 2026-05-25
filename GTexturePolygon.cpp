#include "GTexturePolygon.h"


GTexturePolygon::GTexturePolygon(std::vector<GTextureTriangle> triangles, uint32_t textureID) : triangles(triangles), textureID(textureID) {}

GTexturePolygon::GTexturePolygon(int newHeight, int newWidth, uint32_t textureID) : textureID(textureID) {
    this->triangles = {
        GTextureTriangle{{
            GTextureVertex{GVector<4>{0,0,0,1}, GVector<2>{0,0}},
            GTextureVertex{GVector<4>{(float)newWidth,0,0,1}, GVector<2>{1,0}},
            GTextureVertex{GVector<4>{(float)newWidth,(float)newHeight,0,1}, GVector<2>{1,1}}
        }},
        GTextureTriangle{{
            GTextureVertex{GVector<4>{0,0,0,1}, GVector<2>{0,0}},
            GTextureVertex{GVector<4>{(float)newWidth,(float)newHeight,0,1}, GVector<2>{1,1}},
            GTextureVertex{GVector<4>{0,(float)newHeight,0,1}, GVector<2>{0,1}}
        }}
    };

}

GTexturePolygon::GTexturePolygon(const GTexturePolygon& other) : triangles(other.triangles), textureID(other.textureID) {}

GTexturePolygon& GTexturePolygon::applyTransformation(GMatrix<4,4> transformMatrix) {
    for (size_t i=0;i<triangles.size();i++) {
        for (size_t j=0;j<3;j++) { // Triangles are always 3 vertices
            triangles[i].vertices[j].worldVertex = transformMatrix*triangles[i].vertices[j].worldVertex;
        }
    }

    return *this;
}