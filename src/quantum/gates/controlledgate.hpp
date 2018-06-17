#ifndef _QLIB_QUANTUM_CONTROLLEDGATE_H
#define _QLIB_QUANTUM_CONTROLLEDGATE_H

#include "./igate.hpp"
#include "onequbitgate.hpp"

namespace qlib {
namespace quantum {
namespace gates {

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class controlledgate : public igate {

    private:
        std::string sname;
        onequbitgate& refgate;

    public:
        /// <Summary>
        /// Create a new quanutm operator for the given number of states
        /// </Summary>
        controlledgate(std::string name, onequbitgate& gate) : sname(name), refgate(gate){}

        /// <Summary>
        /// Number of input qubits this operator can be applied to
        /// </Summary>
        size_t inputs(){
            return 2;
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
        void operate(matrix& in, matrix& out, std::vector<ulong> inputQubits){
            matrix& m = refgate.getMatrix();

            //Init loop
            ulong k = inputQubits[1];
            ulong c = 1 << inputQubits[0];
            ulong stride = 1 << k;
            ulong stride_gap = stride << 1;
            
            //Iterate over consecutive groups of amplitudes
            for(ulong g = 0; g < in.countRows(); g += stride_gap){
                //Apply Q to every pair of amplitudes where the control bit is set
                for(ulong i = g; i < g + stride; i++){
                    if(i & c == 0)
                        continue;
                    complex ai      = m(0,0) * in(i,0) + m(0,1) * in(i + stride,0);
                    complex ai_2k   = m(1,0) * in(i,0) + m(1,1) * in(i + stride,0);

                    out(i,0) = ai;
                    out(i + stride, 0) = ai_2k;
                }
            }
        }

        /// <Summary>
        /// Returns the matrix representation of this operator
        /// </Summary>
        qlib::math::matrix& getMatrix(){
            return this->refgate.getMatrix();
        }

        /// <Summary>
        /// Pretty print
        /// </Summary>
        std::string toString(){
            std::stringstream sb;

            sb << (this->sname) << "(" << (2) << ") -" << (this->refgate.toString());

            return sb.str();
        }
};

}
}
}

#endif