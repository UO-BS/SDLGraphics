#include "GEngine/WorldPosition.h"
#include <vector>
#include "GEngine/GVector.h"
#include "GEngine/GMatrix.h"

WorldPosition::WorldPosition(): state(GMatrix<4,4>{std::array<float,16>{1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}}) {}

WorldPosition::WorldPosition(GMatrix<4,4> givenState): state(givenState) {}

WorldPosition::WorldPosition(GMatrix<3,3> givenRotation, GVector<3> givenLocation) {
    state = GMatrix<4,4>{std::array<float,16>{
        givenRotation[0],
        givenRotation[1],
        givenRotation[2],
        0,
        givenRotation[3],
        givenRotation[4],
        givenRotation[5],
        0,
        givenRotation[6],
        givenRotation[7],
        givenRotation[8],
        0,
        givenLocation[0],
        givenLocation[1],
        givenLocation[2],
        1}};
}



void WorldPosition::translate(GVector<3> translation, bool absolute) {
    if (!absolute) {
        GVector<4> translation4d = state*(translation.extendDimension(0));
        state[12] += translation4d[0];
        state[13] += translation4d[1];
        state[14] += translation4d[2];
    } else {
        state[12] += translation[0];
        state[13] += translation[1];
        state[14] += translation[2];
    }
}
void WorldPosition::setLocation(GVector<3> givenLocation) {
    state[12] = givenLocation[0];
    state[13] = givenLocation[1];
    state[14] = givenLocation[2];
}
GVector<3> WorldPosition::getLocation() {
    return GVector<3>{state[12],state[13],state[14]};
}



void WorldPosition::rotate(GMatrix<3,3> rotation) {
    GMatrix<4,4> rotation4d = GMatrix<4,4>{std::array<float,16>{
        rotation[0],
        rotation[1],
        rotation[2],
        0,
        rotation[3],
        rotation[4],
        rotation[5],
        0,
        rotation[6],
        rotation[7],
        rotation[8],
        0,
        0,
        0,
        0,
        1}};

    transform(rotation4d);
}
void WorldPosition::rotate(float xRot, float yRot, float zRot) {
    float roll = xRot*(3.14159/180);
    float pitch = yRot*(3.14159/180);
    float yaw = zRot*(3.14159/180);

    GMatrix<4,4> rotation4d = GMatrix<4,4>{std::array<float,16>{
        cos(pitch)*cos(yaw), 
        cos(pitch)*sin(yaw), 
        -sin(pitch),    
        0,      
        sin(roll)*sin(pitch)*cos(yaw)-cos(roll)*sin(yaw), 
        sin(roll)*sin(pitch)*sin(yaw)+cos(roll)*cos(yaw), 
        sin(roll)*cos(pitch),                            
        0,
        cos(roll)*sin(pitch)*cos(yaw)+sin(roll)*sin(yaw), 
        cos(roll)*sin(pitch)*sin(yaw)-sin(roll)*cos(yaw), 
        cos(roll)*cos(pitch),                                
        0,                                                     
        0,
        0,
        0,
        1}};

    transform(rotation4d);
}

void WorldPosition::setRotation(GMatrix<3,3> givenLocation) {
    state[0] = givenLocation[0];
    state[1] = givenLocation[1];
    state[2] = givenLocation[2];
    state[4] = givenLocation[3];
    state[5] = givenLocation[4];
    state[6] = givenLocation[5];
    state[8] = givenLocation[6];
    state[9] = givenLocation[7];
    state[10] = givenLocation[8];
}
GMatrix<3,3> WorldPosition::getRotation() {
    return GMatrix<3,3>{std::array<float,9>{state[0],state[1],state[2],state[4],state[5],state[6],state[8],state[9],state[10]}};
}



void WorldPosition::transform(GMatrix<4,4> transformation, bool absolute) {
    if (absolute) {
        state = transformation * state;
    } else {
        state *= transformation;
    }
}

const GMatrix<4,4>& WorldPosition::getWorldPosition() const {
    return state;
}
