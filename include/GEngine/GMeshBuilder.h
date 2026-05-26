#ifndef G_MESH_BUILDER_H
#define G_MESH_BUILDER_H

#include <vector>
#include "GEngine/GCommonStructs.h"


// TODO: Come up with a better way to add complex meshes... Loading in a file perhaps?
namespace GMeshBuilder {

    // Generates a mesh for a rectangle (The origin point is in the center of the rectangle)
    std::vector<GTextureVertex> CreateRectangle(float width, float height);

    // Generates a mesh for a cube
    // The origin point (0,0,0) will sit exactly in the center of the cube.
    std::vector<GTextureVertex> CreateCube(float width, float height, float depth);

    // Generates a mesh for a cube with 6 different faces (3 in row 1, 3 in row 2)
    // The origin point (0,0,0) will sit exactly in the center of the cube.
    std::vector<GTextureVertex> CreateCubeWithFaces(float width, float height, float depth);

}


#endif