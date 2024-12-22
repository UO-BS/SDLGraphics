#ifndef G_QUATERNION_H
#define G_QUATERNION_H

#include "GVector.h"
#include "GMatrix.h"
#include <cmath>

class GQuaternion
{
public:

    // In the form x,y,z,w
    GVector<4> vec;

    GQuaternion(GVector<4> dataVec) : vec{dataVec} {}

    GQuaternion(GVector<3> axis, float angle) {
        angle = angle*(3.14159/180);

        vec[0] = axis[0]*sin(angle/2);
        vec[1] = axis[1]*sin(angle/2);
        vec[2] = axis[2]*sin(angle/2);
        vec[3] = cos(angle/2);
    }

    float& operator[](size_t index)
    {
        return vec[index];
    }

    GQuaternion(float roll, float pitch, float yaw){
        roll = roll*(3.14159/180);
        pitch = pitch*(3.14159/180);
        yaw = yaw*(3.14159/180);

        vec[0] = sin(roll/2)*cos(pitch/2)*cos(yaw/2) - cos(roll/2)*sin(pitch/2)*sin(yaw/2);
        vec[1] = cos(roll/2)*sin(pitch/2)*cos(yaw/2) + sin(roll/2)*cos(pitch/2)*sin(yaw/2);
        vec[2] = cos(roll/2)*cos(pitch/2)*sin(yaw/2) - sin(roll/2)*sin(pitch/2)*cos(yaw/2);
        vec[3] = cos(roll/2)*cos(pitch/2)*cos(yaw/2) + sin(roll/2)*sin(pitch/2)*sin(yaw/2);
    }

    //Active rotation: Point is rotated about axis
    //Passive rotation: Axis is rotated about point (opposite rotation direction)
    GVector<3> performRotation(GVector<3> v, bool active = true){
        GQuaternion point{GVector<4>{v[0],v[1],v[2],0.0f}};
        if (active) {
            GQuaternion result = ((this->getInverse())*point)*(*this);
            return GVector<3>{result[0],result[1],result[2]};
        } else {
            GQuaternion result = ((*this)*point)*(this->getInverse());
            return GVector<3>{result[0],result[1],result[2]};
        }
    }

    GQuaternion operator*(const GQuaternion& q) {
        GQuaternion result{GVector<4>{
            //w,x,y,z
            (this->vec[3]*q.vec.data[0]) + (this->vec[0]*q.vec.data[3]) - (this->vec[1]*q.vec.data[2]) + (this->vec[2]*q.vec.data[1]),
            (this->vec[3]*q.vec.data[1]) + (this->vec[0]*q.vec.data[2]) + (this->vec[1]*q.vec.data[3]) - (this->vec[2]*q.vec.data[0]),
            (this->vec[3]*q.vec.data[2]) - (this->vec[0]*q.vec.data[1]) + (this->vec[1]*q.vec.data[0]) + (this->vec[2]*q.vec.data[3]),
            (this->vec[3]*q.vec.data[3]) - (this->vec[0]*q.vec.data[0]) - (this->vec[1]*q.vec.data[1]) - (this->vec[2]*q.vec.data[2])
        }};
        
        return result;
    }

    GQuaternion getInverse(){
        GQuaternion result{GVector<4>{-vec[0],-vec[1],-vec[2],vec[3]}};
        return result;
    }

    GMatrix<3,3> get3RotMat() {
        GMatrix<3,3> result;

        result[0] = 1 - 2*vec[1]*vec[1] - 2*vec[2]*vec[2];
        result[1] = 2*vec[0]*vec[1] - 2*vec[3]*vec[2];
        result[2] = 2*vec[0]*vec[2] + 2*vec[3]*vec[1];

        result[3] = 2*vec[0]*vec[1] + 2*vec[3]*vec[2];
        result[4] = 1 - 2*vec[0]*vec[0] - 2*vec[2]*vec[2];
        result[5] = 2*vec[1]*vec[2] - 2*vec[3]*vec[0];
        
        result[6] = 2*vec[0]*vec[2] - 2*vec[3]*vec[1];
        result[7] = 2*vec[1]*vec[2] + 2*vec[3]*vec[0];
        result[8] = 1 - 2*vec[0]*vec[0] - 2*vec[1]*vec[1];

        return result;
    }

    GMatrix<4,4> get4RotMat() {
        GMatrix<4,4> result;

        result[0] = 1 - 2*vec[1]*vec[1] - 2*vec[2]*vec[2];
        result[1] = 2*vec[0]*vec[1] - 2*vec[3]*vec[2];
        result[2] = 2*vec[0]*vec[2] + 2*vec[3]*vec[1];
        result[3] = 0;

        result[4] = 2*vec[0]*vec[1] + 2*vec[3]*vec[2];
        result[5] = 1 - 2*vec[0]*vec[0] - 2*vec[2]*vec[2];
        result[6] = 2*vec[1]*vec[2] - 2*vec[3]*vec[0];
        result[7] = 0;
        
        result[8] = 2*vec[0]*vec[2] - 2*vec[3]*vec[1];
        result[9] = 2*vec[1]*vec[2] + 2*vec[3]*vec[0];
        result[10] = 1 - 2*vec[0]*vec[0] - 2*vec[1]*vec[1];
        result[11] = 0;

        result[12] = 0;
        result[13] = 0;
        result[14] = 0;
        result[15] = 1;

        return result;
    }


};

#endif