#ifndef ENTITY_H
#define ENTITY_H

#include "WorldPosition.h"
#include "GMesh.h"

// This class represents an object
// This is a temporary class, for development testing only

class Entity
{
public:

    // Position in world space
    WorldPosition pos;
    // The Graphical component of the object
    const GMesh* graphics = nullptr;

    Entity(const GMesh* meshPtr) : pos(WorldPosition{}), graphics(meshPtr) {}

    // Calls the graphics component to draw the object
    void draw(int shaderMatrixLocation) const {
        if (graphics == nullptr) return;

        // Tell the shader where the object is located
        glUniformMatrix4fv(shaderMatrixLocation, 1, GL_FALSE, pos.getWorldPosition().getRaw());

        // Trigger the drawing process
        graphics->draw();
    }
    
private:


};

#endif