#ifndef G_GRAPHICAL_COMPONENT_H
#define G_GRAPHICAL_COMPONENT_H

#include <vector>
#include "GMatrix.h"
#include "GVector.h"
#include "GTexturePolygon.h"

/*
    This class represents the GraphicalComponent of an object in 3D space
    It holds a collection of textures and their positions/rotations relative to the object (Model space)
    For example; a cube has 6 'polygons' and the 'polygonPosRot' is what arranges the 6 sides into the cube shape

    I use this convention: https://stackoverflow.com/questions/10094634/4x4-matrix-last-element-significance
    The corner 3x3 matrix is the rotation, and the 4th column is the position

    TODO: Add visible flag 
*/ 

class GGraphicalComponent
{
public:

    // These polygons form the graphical "object"
    std::vector<GTexturePolygon> polygons;
    std::vector<GMatrix<4,4>> polygonPosRot;

    //constructors
    GGraphicalComponent();
    GGraphicalComponent(std::vector<GTexturePolygon> polygons, std::vector<GMatrix<4,4>> polygonPosRot);

    //Controlling individual polygons
    void addPolygon(GTexturePolygon newPolygon, GMatrix<4,4> newPosRot);

    // Get polygon details
    std::vector<GTexturePolygon> getPolygons();
};

#endif