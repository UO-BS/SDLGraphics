#ifndef G_MATRIX_H
#define G_MATRIX_H

#include <array>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "GVector.h"

template <size_t R, size_t C>
class GMatrix
{
public:

    // The GMatrix is columns of Vectors. GMatrix<4,4>[4] is the first row second column.
    std::array<float,R*C> data;

    //0 Matrix
    GMatrix() : data{}{}
    GMatrix(std::array<float,R*C> matrixData){
        data = matrixData;
    }
    //Forms a matrix from column vectors
    GMatrix(std::array<GVector<R>,C> vectors) : data{}{
        for (int i=0;i<C;i++) {	//each vector
            for (int crd=0;crd<R;crd++) { //each coordinate
                this->data[i*R + crd] = vectors[i].data[crd];
            }
        }
    }

    float& operator[](size_t index)
    {
        if(index>=R*C+C){
            std::out_of_range e("Matrix index out of range");
            throw e;
        }
        return data[index];
    }

    template <size_t R2, size_t C2>
    GMatrix<R,C2> operator*(const GMatrix<R2,C2>& m) const
    {
        if (C!=R2) {
            std::invalid_argument e("Invalid Matrix");
            throw e;
        } else {

            GMatrix<R,C2> result{};

            for (int row1=0;row1<R;row1++) {
                for (int col2=0;col2<C2;col2++) {
                    for (int col1=0;col1<C;col1++) {
                        result[(row1)+C*col2] += this->data[(row1)+C*col1] * m.data[(col1)+C*col2];
                    }
                } 
            }


            return result;

        }
    }

    GMatrix& operator*=(const GMatrix& m)
    {
        *this = (*this) * m;
        return *this;
    }

    GMatrix<R,C> getInverse() {
        
        GMatrix<R,C> inverse;

        if (R != C) {
            throw std::invalid_argument ("Inverse requires square matrix");
        } else {
            int det = getDeterminant(*this,R);
            if (det==0){
                std::invalid_argument e("Determinant is 0, no inverse possible");
                throw e;
            }
            
           inverse = (this->getCofactor()).getTranspose();
    
           for (int i=0;i<R*C;i++) {
                inverse[i]/=det;
           }
        }
        return inverse;
    }

    GMatrix<R,C> getTranspose() {

        GMatrix<R,C> solution{};

        for(size_t i = 0; i < R; i++) {
            for(size_t j = 0; j < C; j++) {
                solution[j*R +i] = this->data[i*R +j];
            }
        }
        return solution;
    }


    GMatrix<R,C> getCofactor() 
    {
        if(R != C) {
            throw std::invalid_argument ("Cofactor requires square matrix");
        } 

        GMatrix<R,C> solution{};
        GMatrix<R-1,C-1> subVect{};

        for(std::size_t i = 0; i < R; i++) {
            for(std::size_t j = 0; j < C; j++) {

                int p = 0;
                for(size_t x = 0; x < R; x++) {
                    if(x == i) {
                        continue;
                    }
                    int q = 0;

                    for(size_t y = 0; y < R; y++) {
                        if(y == j) {
                            continue;
                        }

                        subVect[p*(R-1) +q] = this->data[x*R +y];
                        q++;
                    }
                    p++;
                }
                solution[i*R +j] = pow(-1, i + j) * getDeterminant(subVect,R-1);
            }
        }
        return solution;
    }

    // Recursive function to find determinant of a matrix
    // n must be equal to A for a useful result. So for a 3x3 matrix, n=3
    template <size_t A, size_t B>
    static int getDeterminant(GMatrix<A,B> mat, int n)
    {
        if (A != B) {
            throw std::invalid_argument ("determinant requires square matrix");
        }
        if (n > A) {
            throw std::invalid_argument ("Invalid n value. n is too large (n > dimension of matrix)");
        }
        // Initialize result
        int num1, num2, det = 1, index, total = 1;
        // Temporary array for storing row
        int temp[n + 1];
        // Loop for traversing the diagonal elements
        for (int i = 0; i < n; i++)
        {
            // Initialize the index
            index = i;
            // Finding the index which has non zero value
            while (index < n && mat[index*n +i] == 0){index++;}
    
            // if there is non zero element
            if (index == n)
            {
                // the determinant of matrix as zero
                continue;
            }
            if (index != i)
            {
                // Loop for swapping the diagonal element row and index row
                for (int j = 0; j < n; j++)
                {
                    //Perform swap
                    float temp = mat[i*n +j];
                    mat[i*n +j] = mat[index*n +j];
                    mat[index*n +j] = temp;
                }
    
                // Determinant sign changes when we shift rows go through determinant properties
                det = det * pow(-1, index - i);
            }
    
            // Storing the values of diagonal row elements
            for (int j = 0; j < n; j++)
            {
                temp[j] = mat[i*n +j];
            }
    
            // Traversing every row below the diagonal element
            for (int j = i + 1; j < n; j++)
            {
                // Value of diagonal element
                num1 = temp[i];
    
                // Value of next row element
                num2 = mat[j*n +i];
    
                // Traversing every column of row and multiplying to every row
                for (int k = 0; k < n; k++)
                {
                    // Multiplying to make the diagonal element and next row element equal
                    mat[j*n +k] = (num1 * mat[j*n +k]) - (num2 * temp[k]);
                }
                total = total * num1; // Det(kA)=kDet(A);
            }
        }
    
        // Multiplying the diagonal elements to get determinant
        for (int i = 0; i < n; i++)
        {
            det = det * mat[i*n +i];
        }
    
        // Det(kA)/k=Det(A);
        return (det / total);
    }

};

template <size_t R, size_t C, size_t D>
inline GVector<D> operator*(const GMatrix<R,C>& m, const GVector<D>& v)
{
    if (D!=C) {
        std::invalid_argument e("Incompatible matrix and Vector");
        throw e;
    } else {
        GVector<D> result;
        //For each row in the vector
        for (size_t i=0;i<D;i++) {
            //For each row of the matrix
            for (size_t j=0;j<R;j++) {
                result[j] += v.data[i] * m.data[(j)+i*C];
            }
            
        }
        return result;
    }
    
}

template <size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const GMatrix<R,C>& matrix) {
    os << "Matrix(";
    for (size_t i = 0; i<R*C ;i++) {
        os << matrix.data[i];
        if (i==R*C-1) {break;}
        os << ",";
    }
    os << ")";
    return os;
}

#endif