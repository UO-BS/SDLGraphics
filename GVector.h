#ifndef G_VECTOR_H
#define G_VECTOR_H

#include <cmath>
#include <iostream>
#include <array>
#include <stdexcept>

template <size_t T>
class GVector
{
public:

    std::array<float,T> data;

    GVector() : data{}{}
    GVector(std::array<float,T> given) {
        this->data = given;
    }
    template <typename...M>
    GVector(M... values) : data{values...} {}

    float& operator[](size_t index)
    {
        if(index>=T){
            std::out_of_range e("Index out of range");
            throw e;
        }
        return data[index];
    }

    const float& operator[](size_t index) const
    {
        if(index>=T){
            std::out_of_range e("Index out of range");
            throw e;
        }
        return data[index];
    }

    float length() const {
        float total=0;
        for (int i=0;i<T;i++) {
            total += pow(data[i],2);
        }
        return sqrt(total);
    }

    GVector<T+1> extendDimension(float nextValue) {
        std::array<float,T+1> newGVector;
        std::copy(std::begin(this->data), std::end(this->data), std::begin(newGVector));
        newGVector[T] = nextValue;
        return newGVector;
    }

    GVector unitV() const {
        float mag = length();
        std::array<float,T> dir;
        for (int i=0;i<T;i++) {
            dir[i] = this->data[i]/mag;
        }
        return GVector(dir);
    }

    GVector<T> operator-(const GVector<T>& o) const
    {
        GVector<T> result{};
        for (int i=0;i<data.size();i++) {
            result[i] = data[i] - o.data[i];
        }

        return result;
    }

    GVector<T> operator+(const GVector<T>& o) const
    {
        GVector<T> result{};
        for (int i=0;i<data.size();i++) {
            result[i] = data[i] + o.data[i];
        }

        return result;
    }

    GVector operator*(int o) const
    {
        GVector result{};
        for (int i=0;i<data.size();i++) {
            result[i] = data[i] * o;
        }

        return result;
    }
    
    //Find the intersection of two vectors given their 2 unit vectors and two points
    static GVector<T> intersectPoint(const GVector<T>& vec1, const GVector<T>& vec1P, const GVector<T>& vec2, const GVector<T>& vec2P ) {
        //NEED TO ENSURE THAT vec1 AND vec2 are unit vectors
        //ALSO THIS NEEDS DOCUMENTATION

        GVector<T> resultVector;

        GVector<T> bVal = vec2P - vec1P;
        float tStatic;
		float tDynamic;
        float tStatic2;
		float tDynamic2;
		float Smultiplier;
		float Tmultiplier;

        // We are trying to find the point at which 2 unit vectors intersect.
		//bVal + topRightUnit*S = botRightUnit*T      (for all coordinates) so:
		//bVal[0] + topRightUnit[0]*S = botRightUnit[0]*T
		//bVal[1] + topRightUnit[1]*S = botRightUnit[1]*T
		//bVal[2] + topRightUnit[2]*S = botRightUnit[2]*T

        std::array<int,T> pairZeroCounts;
		for (int pair=0;pair<T;pair++) {
			pairZeroCounts[pair] = ((vec1[pair]==0)?1:0) + ((vec2[pair]==0)?2:0);
			// 0 : both are non-zero
			// 1 : vec1 is zero
			// 2 : vec2 is zero
			// 3 : both are zero
		}

		//For each of the X,Y,Z value pairs; if only one of the vectors has a coord==0, then we can take a shortcut. 
		bool usedShortcut = false;
		for (int pair=0;pair<T;pair++) {
			if (pairZeroCounts[pair] == 1) {
				Smultiplier = -bVal[pair]/vec2[pair];

                resultVector = vec2P + (vec2*Smultiplier);

				usedShortcut = true;
				break;
			} else if (pairZeroCounts[pair] == 2) {
				Tmultiplier = bVal[pair]/vec1[pair];

                resultVector = vec1P + (vec1*Tmultiplier);
				
				usedShortcut = true;
				break;
			}
		}

		//If we did not take the shortcut, we have to calculate the long way
		if (!usedShortcut) {
			bool staticAndDynamicPrepared = false;
			for (int pair=0;pair<T;pair++) {
				if (pairZeroCounts[pair] == 0) {
					if (!staticAndDynamicPrepared) {
						//bVal + vec2*S = vec1*T 
						//Therefore T = ( bVal[0] + vec2[0]*S )/vec1[0]    
						//Therefore T = ( bVal[0]/vec1[0]) + (vec2[0]/vec1[0])*S 
						tStatic = ( bVal[pair]/vec1[pair]);
						tDynamic = (vec2[pair]/vec1[pair]);
						//T = tStatic + tDynamic*S

						staticAndDynamicPrepared = true;
					} else {
						//Therefore bVal[1] + vec2[1]*S = vec1[1]*(tStatic + tDynamic*S)
						tStatic2 = tStatic * vec1[pair];
						tDynamic2 = tDynamic * vec1[pair];
						//Now:   bVal[1] + vec2[1]*S = tStatic + tDynamic*S
						//Therfore:     (bVal[1] - tStatic)  =  (tDynamic - vec2[1])*S
						//Therefore:    S = (bVal[1] - tStatic) / (tDynamic - vec2[1])
						Smultiplier = (bVal[pair] - tStatic2) / (tDynamic2 - vec2[pair]);
						//T = ( bVal[0] + vec2[0]*S )/vec1[0];
                        //Tmultiplier = ( bVal[pair] + vec2[pair]*Smultiplier )/vec1[pair];
					}
				}
			}
            resultVector = vec2P + (vec2*Smultiplier);
		}
        
        return resultVector;
    }
    

};

template <size_t T>
std::ostream& operator<<(std::ostream& os, const GVector<T>& vector) {
    os << "Vector(";
    for (size_t i = 0; i<T ;i++) {
        os << vector.data[i];
        if (i==T-1) {break;}
        os << ",";
    }
    os << ")";
    return os;
}

#endif