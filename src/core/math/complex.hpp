#ifndef _QLIB_MATH_COMPLEX_H
#define _QLIB_MATH_COMPLEX_H

#include "./../general/object.hpp"

#include <math.h> 
#include <stdlib.h>
#include <sstream>

namespace qlib {
namespace math {

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

/// <Summary>
/// Object representation of a complex number in the form a + ib;
/// </Summary>
class complex : public xobject {

    private:

    public:
        /// </Summary>
        /// Real component
        /// </Summary>
        f32 r;
        /// </Summary>
        /// Imaginary component
        /// </Summary>
        f32 i;

        /// </Summary>
        /// Create a complex number representing 0 + 0i
        /// </Summary>
        complex(){
            r = 0;
            i = 0;
        };

        /// </Summary>
        /// Create a complex number representing real + 0i
        /// </Summary>
        complex(f32 real){
            this->r = real;
            this->i = 0;
        };

        /// </Summary>
        /// Create a complex number representing real + img i
        /// </Summary>
        complex(f32 real, f32 img){
            this->r = real;
            this->i = img;
        };
        
        /// </Summary>
        /// Create a complex number from another
        /// </Summary>
        complex(const complex& other){
            this->r = other.r;
            this->i = other.i;
        };

        /// </Summary>
        /// Squared magnitude of this complex number equals this * this->conjugate(). This is purely a real number.
        /// </Summary>
        f32 sqrMagnitude(){
            complex b = this->conjugate();
            return this->r * b.r - this->i * b.i;
        };

        /// </Summary>
        /// Magnitude of this complex number equals sqrt(abs(square magnitude))
        /// </Summary>
        f32 magnitude(){
            return (f32)sqrt(abs(sqrMagnitude()));
        };

        /// </Summary>
        /// Returns the complex conjugate of this number
        /// </Summary>
        complex conjugate(){
            return complex(this->r, -1 * this->i);
        };

        /// </Summary>
        /// Returns a complex number of unit length but the same direction in the complex plane
        /// </Summary>
        complex normal(){
            f32 mag = this->magnitude();
            return complex(this->r / mag, this->i / mag);
        };

        /// </Summary>
        /// Checks if this complex number can be represented as a purely real number
        /// </Summary>
        bool isPureReal(){
            return this->i == 0;
        };

        /// </Summary>
        /// Checks if this complex number can be represented as a purely imaginary number
        /// </Summary>
        bool isPureImaginary(){
            return this->r == 0;
        };

        /// </Summary>
        /// Checks if this complex number is zero
        /// </Summary>
        bool isZero(){
            return this->r == 0 && this->i == 0;
        };

        /// </Summary>
        /// Complex argument
        /// </Summary>
        f32 arg(){
            return (f32)atan2(this->i, this->r);
        };

        /// </Summary>
        /// String representation
        /// </Summary>
        std::string toString(){
            std::stringstream sb;
            sb << this->r << " + " << this->i << "i";
            return sb.str();
        };
};

//----------------------------------------------------------
// Operators
//----------------------------------------------------------

/// </Summary>
/// Add two complex numbers
/// </Summary>
complex operator + (complex a, complex b){
    return complex(a.r + b.r, a.i + b.i);
};

/// </Summary>
/// Subtract two complext numbers
/// </Summary>
complex operator - (complex a, complex b){
    return complex(a.r - b.r, a.i - b.i);
};

/// </Summary>
/// Multiply two complex numbers
/// </Summary>
complex operator * (f32 s, complex a){
    return complex(s * a.r, s * a.i);
};

/// </Summary>
/// Scale up a complex vector by a scalar quantity
/// </Summary>
complex operator * (complex a, f32 s){
    return complex(s * a.r, s * a.i);
};

/// </Summary>
/// Scale down a complex vector by a scalar quantity
/// </Summary>
complex operator / (complex a, f32 s){
    return complex(a.r / s, a.i / s);
};

/// </Summary>
/// Multiply two complex vectors
/// </Summary>
complex operator * (complex a, complex b){
    return complex(a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r);
};

/// </Summary>
/// Divide two complex vectors
/// </Summary>
complex operator / (complex a, complex b){
    f32 d = b.r * b.r + b.i * b.i;
    f32 r = (a.r * b.r + a.i * b.i) / d;
    f32 i = (a.i * b.r - a.r * b.i) / d;
    return complex(r, i);
}

}
}

#endif