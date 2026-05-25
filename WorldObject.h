#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "WorldPosition.h"
#include "GGraphicalComponent.h"

// This class represents an object
// This is a temporary class, for development testing only

class WorldObject
{
public:

    // Position in world space
    WorldPosition pos;
    // Graphical component of the object (Model space)
    GGraphicalComponent graphics;

    WorldObject(): pos(WorldPosition{}), graphics(GGraphicalComponent{}) {}
    WorldObject(WorldPosition pos, GGraphicalComponent graphics): pos(pos), graphics(graphics) {}
    
private:


};

#endif