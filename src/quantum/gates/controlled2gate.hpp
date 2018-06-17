#ifndef _QLIB_QUANTUM_CONTROLLED2GATE_H
#define _QLIB_QUANTUM_CONTROLLED2GATE_H

#include "./igate.hpp"
#include "onequbitgate.hpp"
#include <sstream>

namespace qlib {
namespace quantum {
namespace gates {

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class controlled2gate : public igate {

    private:
        std::string sname;
        onequbitgate& refgate;

    public:
        /// <Summary>
        /// Create a new quanutm operator for the given number of states
        /// </Summary>
        controlled2gate(std::string name, onequbitgate& gate) : sname(name), refgate(gate){}

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
            if(inputQubits.size() != 3){
                std::stringstream sb;
                sb << "Three qubit gates operate on only three qubits, ";
                sb << inputQubits.size();
                sb << " provided";
                throw std::length_error(sb.str()); 
            }
            
            matrix& m = refgate.getMatrix();

            //Init loop --first 2 inputs are control
            ulong k = inputQubits[2];
            ulong c1 = inputQubits[0];
            ulong c2 = 1 << inputQubits[1];
            ulong stride = 1 << k;

            for(ulong i = 0; i < in.countRows(); i++){
                ulong mask = i & c1;
                ulong mask2 = i & c2;
                ulong smask = i & stride; 
                if(!mask || !mask2){
                    continue;
                }
                ulong c1_t0 = i & ~smask; //a(*1c*0t*...)
                ulong c1_t1 = i | smask;  //a(*1c*1t*...)

                out(c1_t0, 0) = m(0,0) * in(c1_t0,0) + m(0,1) * in(c1_t1,0);
                out(c1_t1, 0) = m(1,0) * in(c1_t0,0) + m(1,1) * in(c1_t1,0);
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