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

    // The GMatrix is columns of Vectors (column-major). GMatrix<4,4>[4] is the first-row second-column. index = row + (NUM_PER_ROW x col)
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

    const float* getRaw() const {
        return data.data();
    };

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
        } 

        GMatrix<R,C2> result{};

        // For each column in Matrix2
        for (size_t col2 = 0; col2 < C2; col2++) {
            // For each row in Matrix1
            for (size_t row1 = 0; row1 < R; row1++) {
                // For each element to dot-product
                for (size_t col1 = 0; col1 < C; col1++) {
                    result.data[row1 + (R * col2)] += this->data[row1 + (R * col1)] * m.data[col1 + (R2 * col2)];
                }
            }
        }

        return result;

    }

    GMatrix& operator*=(const GMatrix& m)
    {
        *this = (*this) * m;
        return *this;
    }

    GMatrix<R,C> getInverse() const {
        
        GMatrix<R,C> inverse;

        if (R != C) {
            throw std::invalid_argument ("Inverse requires square matrix");
        } else {
            float det = getDeterminant(*this,R);
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

    GMatrix<R,C> getTranspose() const {

        GMatrix<R,C> solution{};

        for(size_t i = 0; i < R; i++) {
            for(size_t j = 0; j < C; j++) {
                solution[j*R +i] = this->data[i*R +j];
            }
        }
        return solution;
    }


    GMatrix<R,C> getCofactor() const {
        if(R != C) {
            throw std::invalid_argument ("Cofactor requires square matrix");
        } 

        GMatrix<R,C> solution{};
        GMatrix<R-1,C-1> subMat{};

        // In the following block of code, 
        // j refers to the column of the original matrix, i refers to the row of the original matrix             --- Used for keeping track of which matrix minor we are calculating
        // pO refers to the column of the original matrix, qO refers to the row of the original matrix           --- Used for indexing the original matrix
        // pS refers to the column of the generated sub-matrix, qS refers to the row of the generated sub-matrix --- Used for indexing the sub-matrix
        
        // Loop through every element of the original matrix to find the minor value
        for(std::size_t i = 0; i < R; i++) {
            for(std::size_t j = 0; j < C; j++) {

                int pS = 0;
                for(size_t pO = 0; pO < C; pO++) { // Loop through every column
                    if(pO == j) {
                        continue; // Skip the minor's column (doesnt increment pS)
                    }
                    int qS = 0;

                    for(size_t qO = 0; qO < R; qO++) { // Loop through every row
                        if(qO == i) {
                            continue; // Skip the minor's row (doesnt increment qS)
                        }

                        subMat[pS*(R-1) +qS] = this->data[pO*C +qO];
                        qS++;
                    }
                    pS++;
                }

                // Calculate the minor value and store it in "solution"
                solution[j*C +i] = pow(-1, i + j) * getDeterminant(subMat,R-1);
                // Clear sub-matrix values (possible optimization: clear only the skipped row/column of next loop; the others will be overwritten)
                for (int clear=0;clear<(R-1)*(C-1);clear++) {subMat[clear]=0;}
            }
        }
        return solution;
    }

    // Function to find determinant of a matrix using Gaussian elimination and a series of transformations that reduce the matrix to upper triangular form
    // Based on: https://www.geeksforgeeks.org/dsa/determinant-of-a-matrix/  
    //     - although slightly different: I use floats, and the "index" and "i" variable are different (it seems the tutorial uses row-major)
    // n must be equal to A for a useful result. So for a 3x3 matrix, n=3
    template <size_t A, size_t B>
    static float getDeterminant(GMatrix<A,B> mat, int n)
    {
        if (A != B) {
            throw std::invalid_argument ("determinant requires square matrix");
        }
        if (n > A) {
            throw std::invalid_argument ("Invalid n value. n is too large (n > dimension of matrix)");
        }
        // Initialize result
        float num1, num2, total = 1, det = 1;
        int index;
        // Temporary array for storing row
        float temp[n + 1]{};
        // Loop for traversing the diagonal elements
        for (int i = 0; i < n; i++) // Note i represents the matrix diagonal; (0,0) (1,1) (2,2) etc...
        {
            // Initialize the index
            index = i; // Note: index represents the row number
            // Finding the index which has non zero value
            while (index < n && mat[i*n +index] == 0){index++;}
    
            // if there is no non zero element in the first column
            if (index == n)
            {
                continue; // the determinant of matrix is zero
            }
            if (index != i)
            {
                // Loop for swapping the diagonal element row and index row
                for (int j = 0; j < n; j++) // Note: j represents the column number
                {
                    //Perform swap
                    float swapTemp = mat[j*n + i];
                    mat[j*n + i] = mat[j*n + index];
                    mat[j*n + index] = swapTemp;
                }
    
                // Determinant sign changes when we shift rows
                det = det * pow(-1, index - i);
            }
    
            // Storing the values of diagonal row elements
            for (int j = 0; j < n; j++) // Note: j represents the column number
            {
                temp[j] = mat[j*n +i];
            }
    
            // Traversing every row below the diagonal element
            for (int bRow = i + 1; bRow < n; bRow++) // Note: bRow represents the row number
            {
                
                num1 = temp[i]; // Value of diagonal element
                num2 = mat[i*n +bRow]; // Value of next row (elements directly below num1)... Elements in the same column as the diagonal element
    
                // Traversing every column of the row and multiplying
                for (int k = 0; k < n; k++) // Note: k represents the column number
                {
                    // Multiplying to make the diagonal element (num1) and next row element (num2) equal... And then subtracting to make next row element zero
                    mat[k*n + bRow] = (num1 * mat[k*n + bRow]) - (num2 * temp[k]);
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