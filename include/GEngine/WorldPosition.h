#ifndef WORLD_POSITION_H
#define WORLD_POSITION_H

#include "GMatrix.h"
#include <vector>

// This class represents an object's state in a world. 
// Holds a 4x4 matrix to represent location and rotation

class WorldPosition
{
public:

    //Creates a 4x4 identity matrix
    WorldPosition();

    WorldPosition(GMatrix<4,4> givenState);
    WorldPosition(GMatrix<3,3> givenRotation, GVector<3> givenLocation);


    //Translate object; The "absolute" variable determines if the object should move relative to the world axis, or relative to where its currently facing
    void translate(GVector<3> translation, bool absolute = true);
    // Set Location
    void setLocation(GVector<3> givenLocation);
    // Get Location
    GVector<3> getLocation();

    //Rotate object; 
    void rotate(GMatrix<3,3> rotation);
    //Rotate by roll pitch yaw (in degrees)
    void rotate(float xRot, float yRot, float zRot);
    // Set rotation
    void setRotation(GMatrix<3,3> givenLocation);
    // Get Rotation
    GMatrix<3,3> getRotation();

    // The "absolute" variable determines if the object should move relative to the world axis, or relative to where its currently facing
    void transform(GMatrix<4,4> transformation, bool absolute = false);
    const GMatrix<4,4>& getWorldPosition() const;

private:

    GMatrix<4,4> state;

};

#endif