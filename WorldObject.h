#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "WorldPosition.h"
#include "GGraphicalComponent.h"

// This class represents an object
// This is a temporary class, for development testing only

class WorldObject
{
public:

    WorldPosition pos;
    GGraphicalComponent graphics;

    WorldObject(): pos(WorldPosition{}), graphics(GGraphicalComponent{}) {}
    WorldObject(WorldPosition pos, GGraphicalComponent graphics): pos(pos), graphics(graphics) {}
    
    // Get the coordinates of the textures in world space.
    std::vector<GMatrix<4,4>> getTextureTruePosRot() {
        return graphics.getTextureTruePosRot(pos.getWorldPosition());
    }

private:


};

#endif