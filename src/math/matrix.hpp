#include "complex.hpp"
#include "./../general/object.hpp"

#include <stdexcept>
#include <vector>
#include <math.h> 
#include <sstream>

#ifndef _QLIB_MATH_MATRIX_H
#define _QLIB_MATH_MATRIX_H

namespace qlib {
namespace math {

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class matrix : xobject {

    private:
        complex values[];
        uint rows;
        uint columns;
        uint length;

    public:
        /// </Summary>
        /// Create an empty matrix of this size
        /// </Summary>
        matrix(int rows, int columns){
            this->rows = rows > 0 ? rows : 1;
            this->columns = columns > 0 ? columns : 1;
            this->length = (this->rows) * (this->columns);
            this->values = complex[this->length];
        }

        /// </Summary>
        /// Create a matrix of this size with the given values in compressed row->column form 
        /// </Summary>
        matrix(int rows, int colums, std::vector<complex> values) : matrix(rows, columns){
            size_t smallest = this->length < values.size() ? this->length : values.size();
            for(size_t i = 0; i < smallest; i++){
                this->values[i] = values[i];
            }
        }

        /// </Summary>
        /// Matrix destructor
        /// </Summary>
        ~matrix(){
            
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex& operator() (int r, int c) {
            int idx = r * columns + c;
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            return values[idx];
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex& operator() (int idx) {
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            return values[idx];
        }

        /// </Summary>
        /// Number of rows
        /// </Summary>
        uint countRows(){
            return this->rows;
        }

        /// </Summary>
        /// Number of columns
        /// </Summary>
        uint countColumns(){
            return this->columns; 
        }

        /// </Summary>
        /// Total number of elements
        /// </Summary>
        uint size(){
            return this->length;
        }

        /// </Summary>
        /// Set element in matrix
        /// </Summary>
        void set(int r, int c, complex value){
            int idx = r * columns + c;
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            this->values[idx] = value;
        }

        /// </Summary>
        /// Set element in matrix
        /// </Summary>
        void set(int idx, complex value){
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            this->values[idx] = value;
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex get(int r, int c){
            int idx = r * columns + c;
            if(idx < 0 || idx >= length){
                throw std::out_of_range("Row or column is out of range");
            }
            return this->values[idx];
        }

        /// </Summary>
        /// Get element from matrix
        /// </Summary>
        complex get(int idx){
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
            sb << "[";
            for(uint r = 0; r < this->rows; r++){
                if(r != 0)
                    sb << ";";
                for(uint c = 0; c < this->columns; c++){
                    if(c != 0)
                        sb << ",";
                    sb << get(r, c).toString();
                }
            }
            sb << "]";
            return sb.str();
        };

};

//----------------------------------------------------------
// Operators
//----------------------------------------------------------

matrix operator + (matrix a, matrix b){
    if(a.countRows() != b.countRows() || a.countColumns() != b.countColumns()){
        throw std::length_error("Matrix dims do not match");
    }
    matrix m(a.countRows(), a.countColumns());
    for(uint i = 0; i < a.size(); i++){
        m.set(i, a.get(i) + b.get(i));
    }
    return m;
}

matrix operator - (matrix a, matrix b){
    if(a.countRows() != b.countRows() || a.countColumns() != b.countColumns()){
        throw std::length_error("Matrix dims do not match");
    }
    matrix m(a.countRows(), a.countColumns());
    for(uint i = 0; i < a.size(); i++){
        m.set(i, a.get(i) - b.get(i));
    }
    return m;
}

matrix operator * (complex s, matrix a){
    matrix m(a.countRows(), a.countColumns());
    for(uint i = 0; i < a.size(); i++){
        m.set(i, a.get(i) * s);
    }
    return m;
}

matrix operator * (matrix a, complex s){
    matrix m(a.countRows(), a.countColumns());
    for(uint i = 0; i < a.size(); i++){
        m.set(i, a.get(i) * s);
    }
    return m;
}

matrix operator / (matrix a, complex s){
    matrix m(a.countRows(), a.countColumns());
    for(uint i = 0; i < a.size(); i++){
        m.set(i, a.get(i) / s);
    }
    return m;
}

matrix operator * (matrix a, matrix b){
    if(a.countColumns() != b.countRows()){
        throw std::length_error("Matrix dims do not match");
    }
    matrix m(a.countRows(), b.countColumns());

    for(uint i = 0; i < a.countRows(); i++){
        for(uint j = 0; j < b.countColumns(); j++){
            complex sum = 0;
            for(uint k = 0; k < a.countColumns(); k++){
                sum = sum + a(i,k) * b(k,j);
            }
            m.set(i,j, sum);
        }
    }

    return m;
}

//Tensor product
matrix operator << (matrix a, matrix b){
	//Compute width
	uint nw = a.countColumns() * b.countColumns();
	uint nh = a.countRows() * b.countRows();
		
	//Create matrix;
	matrix m(nh, nw);
	
	//Loop over all elements setting as necessary
	for(uint ac = 0; ac < a.countColumns(); ac++){
		for(uint ar = 0; ar < a.countRows(); ar++){
			for(uint bc = 0; bc < b.countColumns(); bc++){
				for(uint br = 0; br < b.countRows(); br++){
					complex v = a(ar, ac) * b (br, bc);
					m(ar * b.countRows() + br,ac * b.countColumns() + bc) = v;
				}
			}
		}
	}
	
	return m;
}


}
}

#endif