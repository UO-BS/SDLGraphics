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

// std::move avoids copying the vertices vector to the function (it moves it over). We don't need it for anything after this anyway
GTexturePolygon::GTexturePolygon(std::vector<GTextureVertex> vertices, uint32_t textureID) : textureID(textureID), triangles(convexPolygonToTriangles(std::move(vertices))) {}

void GTexturePolygon::appendConvexPolygon(std::vector<GTextureVertex> vertices) {
    std::vector<GTextureTriangle> newTriangles = convexPolygonToTriangles(std::move(vertices));
    this->triangles.reserve(this->triangles.size() + newTriangles.size()); // Resize in advance to avoid re-allocation
    this->triangles.insert(this->triangles.end(), newTriangles.begin(), newTriangles.end());
}

void GTexturePolygon::removeTriangle(size_t index) {
    if (index < this->triangles.size()) {
        std::swap(this->triangles[index], this->triangles.back());
        triangles.pop_back();

        // This method is slower but preserves the order of the triangles
        //this->triangles.erase(this->triangles.begin() + index);
    }
}

std::vector<GTextureTriangle> GTexturePolygon::convexPolygonToTriangles(std::vector<GTextureVertex> vertices) {
    std::vector<GTextureTriangle> triangles;
    for (size_t i=1;i<vertices.size()-1;i++) {
        triangles.push_back(GTextureTriangle{{
            vertices[0],
            vertices[i],
            vertices[i+1]
        }});
    }
    return triangles;
}