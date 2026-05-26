#include <vector>
#include "GVector.h"
#include "GMatrix.h"
#include "GGraphicalComponent.h"
#include <functional> 

GGraphicalComponent::GGraphicalComponent() : polygons(), polygonPosRot() {}

GGraphicalComponent::GGraphicalComponent(std::vector<GTexturePolygon> polygons, std::vector<GMatrix<4,4>> polygonPosRot) : 
                polygons(polygons), polygonPosRot(polygonPosRot) {}

void GGraphicalComponent::addPolygon(GTexturePolygon newPolygon, GMatrix<4,4> newPosRot){
    polygons.push_back(newPolygon);
    polygonPosRot.push_back(newPosRot);
}
