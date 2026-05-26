#ifndef G_MESH_BUILDER_H
#define G_MESH_BUILDER_H

#include <vector>
#include "GEngine/GCommonStructs.h"

namespace GMeshBuilder {

    // Generates a mesh for a rectangle (The origin point is in the center of the rectangle)
    std::vector<GTextureVertex> CreateRectangle(float width, float height);

    // Generates a mesh for a cube
    // The origin point (0,0,0) will sit exactly in the center of the cube.
    std::vector<GTextureVertex> CreateCube(float width, float height, float depth);

}


#endif