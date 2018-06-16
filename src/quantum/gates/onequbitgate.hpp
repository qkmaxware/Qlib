#ifndef _QLIB_QUANTUM_ONEQUBITGATE_H
#define _QLIB_QUANTUM_ONEQUBITGATE_H

#include "./igate.hpp"


namespace qlib {
namespace quantum {
namespace gates {

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class onequbitgate : public igate {

    private:
        std::string sname;
        matrix m;

    public:
        /// <Summary>
        /// Create a new quanutm operator for the given number of states
        /// </Summary>
        onequbitgate(std::string name, std::vector<complex> values) : m(2, 2, values){
            this->sname = name;
        }

        /// <Summary>
        /// Number of input qubits this operator can be applied to
        /// </Summary>
        size_t inputs(){
            return 1;
        } 

        /// <Summary>
        /// The name of this gate
        /// </Summary>
        std::string name(){
            return this->sname;
        }

        /// <Summary>
        /// Operate on a given column vector state superposition
        /// </Summary>
        matrix operate(matrix& state_vector, std::vector<ulong> inputQubits){
            //Simple implementation for single qubit
            //return (this->m) * state_vector;

            //Init result set by copying the current state
            matrix r = state_vector;
            //Init loop
            ulong k = inputQubits[0];
            ulong stride = 1 << k;
            ulong stride_gap = stride << 1;
            //Iterate over consecutive groups of amplitudes
            for(ulong g = 0; g < state_vector.countRows(); g += stride_gap){
                //Apply Q to every pair of amplitudes
                for(ulong i = g; i < g + stride; i++){
                    complex ai      = m(0,0) * state_vector(i,0) + m(0,1) * state_vector(i + stride,0);
                    complex ai_2k   = m(1,0) * state_vector(i,0) + m(1,1) * state_vector(i + stride,0);

                    r(i,0) = ai;
                    r(i + stride, 0) = ai_2k;
                }
            }
            return r;
        }

        /// <Summary>
        /// Returns the matrix representation of this operator
        /// </Summary>
        qlib::math::matrix& getMatrix(){
            return this->m;
        }

        /// <Summary>
        /// Pretty print
        /// </Summary>
        std::string toString(){
            std::stringstream sb;

            sb << (this->sname) << "(" << (1) << ") -" << (this->m.toString());

            return sb.str();
        }
};

}
}
}

#endif