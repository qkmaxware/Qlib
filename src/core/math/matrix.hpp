#ifndef _QLIB_MATH_MATRIX_H
#define _QLIB_MATH_MATRIX_H

#include "complex.hpp"
#include "./../general/object.hpp"

#include <stdexcept>
#include <vector>
#include <math.h> 
#include <sstream>


namespace qlib {
namespace math {

//----------------------------------------------------------
//Early declarations (before defined)
//----------------------------------------------------------
class matrix;
matrix operator * (matrix a, matrix b);

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

/// </Summary>
/// Complex matrix class
/// </Summary>
class matrix : public xobject {

    private:
        /// </Summary>
        /// Flattened 1d array of matrix values
        /// </Summary>
        std::vector<complex> values;
        /// </Summary>
        /// Number of rows
        /// </Summary>
        size_t rows;
        /// </Summary>
        /// Number of columns
        /// </Summary>
        size_t columns;
        /// </Summary>
        /// Number of elements
        /// </Summary>
        size_t length;

    public:
        /// </Summary>
        /// Create an empty matrix of this size
        /// </Summary>
        matrix(size_t rows, size_t columns){
            this->rows = rows > 0 ? rows : 1;
            this->columns = columns > 0 ? columns : 1;
            this->length = (this->rows) * (this->columns);
            this->values = std::vector<complex>(this->length);
            for(size_t i = 0; i < this->length; i++)
                this->values[i] = complex();
        }

        /// </Summary>
        /// Create a matrix of this size with the given values in compressed row->column form 
        /// </Summary>
        matrix(size_t rows, size_t columns, std::vector<complex> values) : matrix(rows, columns){
            size_t smallest = this->length < values.size() ? this->length : values.size();
            for(size_t i = 0; i < smallest; i++){
                this->values[i] = values[i];
            }
        }

        /// </Summary>
        /// Matrix destructor
        /// </Summary>
        ~matrix(){
            //delete this->values;
        }

        /// </Summary>
        /// Get the matrix representing the transposition of this matrix
        /// </Summary>
        matrix transpose(){
            matrix m(this->columns, this->rows);
            for(size_t r = 0; r < this->rows; r++){
                for(size_t c = 0; c < this->columns; c++){
                    m(c,r) = (*this)(r,c); 
                }
            } 
            return m;
        }

        /// </Summary>
        /// Get the matrix representing the adjoint (complex conjugate transpose) of this matrix
        /// </Summary>
        matrix adjoint(){
            matrix m(this->columns, this->rows);
            for(size_t r = 0; r < this->rows; r++){
                for(size_t c = 0; c < this->columns; c++){
                    m(c,r) = (*this)(r,c).conjugate(); 
                }
            } 
            return m;
        }

        /// </Summary>
        /// Test if this matrix represents a square matrix
        /// </Summary>
        bool isSquare(){
            return (this->countRows() != this->countColumns());
        }

        /// </Summary>
        /// Test if this matrix represents a unitary matrix
        /// </Summary>
        bool isUnitary(){
            matrix a = *this;
            matrix a_dagger = this->adjoint();
            matrix q = a * a_dagger;

            //Is 'q' approximately I
            for(size_t r = 0; r < q.countRows(); r++){
                for(size_t c = 0; c < q.countColumns(); c++){
                    if(r == c){
                        //Approx 1
                        if(!q(r,c).isApproximatelyOne())
                            return false;
                    }else {
                        //Approx 0
                        if(!q(r,c).isApproximatelyZero())
                            return false;
                    }
                }
            }
            return true;
        }

        /// </Summary>
        /// Test if this matrix represents a column matrix
        /// </Summary>
        bool isColumn(){
            return (this->countColumns() == 1);
        }

        /// </Summary>
        /// Test if this matrix represents a row matrix
        /// </Summary>
        bool isRow(){
            return (this->countRows() == 1);
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex& operator() (size_t r, size_t c) {
            size_t idx = r * columns + c;
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            return values[idx];
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex& operator() (size_t idx) {
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            return values[idx];
        }

        /// </Summary>
        /// Number of rows
        /// </Summary>
        size_t countRows(){
            return this->rows;
        }

        /// </Summary>
        /// Number of columns
        /// </Summary>
        size_t countColumns(){
            return this->columns; 
        }

        /// </Summary>
        /// Total number of elements
        /// </Summary>
        size_t size(){
            return this->length;
        }

        /// </Summary>
        /// Set element in matrix
        /// </Summary>
        void set(size_t r, size_t c, complex value){
            size_t idx = r * columns + c;
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            this->values[idx] = value;
        }

        /// </Summary>
        /// Set element in matrix
        /// </Summary>
        void set(size_t idx, complex value){
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            this->values[idx] = value;
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex get(size_t r, size_t c){
            size_t idx = r * columns + c;
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            return this->values[idx];
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex get(size_t idx){
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            return this->values[idx];
        }

        /// </Summary>
        /// String representation
        /// </Summary>
        std::string toString(){
            std::stringstream sb;
            sb << "{";
            for(size_t r = 0; r < this->rows; r++){
                if(r != 0)
                    sb << ";";
                sb << "{";
                for(size_t c = 0; c < this->columns; c++){
                    if(c != 0)
                        sb << ",";
                    sb << get(r, c).toString();
                }
                sb << "}";
            }
            sb << "}";
            return sb.str();
        };

        /// <Summary>
        /// Iterator starting at element 0,0
        /// </Summary>
        std::vector<complex>::iterator begin(){
            return (this->values).begin();
        }
        
        /// <Summary>
        /// Iterator starting after element rows-1,columns-1
        /// </Summary>
        std::vector<complex>::iterator end(){
            return (this->values).end();
        }
};

//----------------------------------------------------------
// Operators
//----------------------------------------------------------

/// </Summary>
/// Add two matrices
/// </Summary>
matrix operator + (matrix a, matrix b){
    if(a.countRows() != b.countRows() || a.countColumns() != b.countColumns()){
        throw std::length_error("Matrix dims do not match");
    }
    matrix m(a.countRows(), a.countColumns());
    for(size_t i = 0; i < a.size(); i++){
        m.set(i, a.get(i) + b.get(i));
    }
    return m;
}

/// </Summary>
/// Subtract two matrices
/// </Summary>
matrix operator - (matrix a, matrix b){
    if(a.countRows() != b.countRows() || a.countColumns() != b.countColumns()){
        throw std::length_error("Matrix dims do not match");
    }
    matrix m(a.countRows(), a.countColumns());
    for(size_t i = 0; i < a.size(); i++){
        m.set(i, a.get(i) - b.get(i));
    }
    return m;
}

/// </Summary>
/// Scale up a matrix by a scalar quantity
/// </Summary>
matrix operator * (complex s, matrix a){
    matrix m(a.countRows(), a.countColumns());
    for(size_t i = 0; i < a.size(); i++){
        m.set(i, a.get(i) * s);
    }
    return m;
}

/// </Summary>
/// Scale up a matrix by a scalar quantity
/// </Summary>
matrix operator * (matrix a, complex s){
    matrix m(a.countRows(), a.countColumns());
    for(size_t i = 0; i < a.size(); i++){
        m.set(i, a.get(i) * s);
    }
    return m;
}

/// </Summary>
/// Scale down a matrix by a scalar quantity
/// </Summary>
matrix operator / (matrix a, complex s){
    matrix m(a.countRows(), a.countColumns());
    for(size_t i = 0; i < a.size(); i++){
        m.set(i, a.get(i) / s);
    }
    return m;
}

/// </Summary>
/// Multiply two matrices
/// </Summary>
matrix operator * (matrix a, matrix b){
    if(a.countColumns() != b.countRows()){
        throw std::length_error("Matrix dims do not match");
    }
    matrix m(a.countRows(), b.countColumns());

    for(size_t i = 0; i < a.countRows(); i++){
        for(size_t j = 0; j < b.countColumns(); j++){
            complex sum = 0;
            for(size_t k = 0; k < a.countColumns(); k++){
                sum = sum + a(i,k) * b(k,j);
            }
            m.set(i,j, sum);
        }
    }

    return m;
}

/// </Summary>
/// Compute the Kronicker tensor product of two matrices
/// </Summary>
matrix operator << (matrix a, matrix b){
	//Compute width
	size_t nw = a.countColumns() * b.countColumns();
	size_t nh = a.countRows() * b.countRows();
		
	//Create matrix;
	matrix m(nh, nw);
	
	//Loop over all elements setting as necessary
    for(size_t ar = 0; ar < a.countRows(); ar++){
        for(size_t ac = 0; ac < a.countColumns(); ac++){
            for(size_t br = 0; br < b.countRows(); br++){
                for(size_t bc = 0; bc < b.countColumns(); bc++){
                    m(br + ar * b.countRows(), bc + ac * b.countColumns()) = a(ar, ac) * b(br, bc); 
                }
            }
        }
    }
	
	return m;
}


}
}

#endif