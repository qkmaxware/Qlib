#ifndef _QLIB_QUANTUM_CONTROLLEDGATE_H
#define _QLIB_QUANTUM_CONTROLLEDGATE_H

#include "./igate.hpp"
#include "./../systems/system.hpp"
#include "onequbitgate.hpp"
#include <sstream>

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
        void operate(u32 qubits, matrix& in, matrix& out, std::vector<u64> inputQubits){
            if(inputQubits.size() != 2){
                std::stringstream sb;
                sb << "Two qubit gates operate on only two qubits, ";
                sb << inputQubits.size();
                sb << " provided";
                throw std::length_error(sb.str()); 
            }

            matrix& m = refgate.getMatrix();

            //Init loop
            u64 k = inputQubits[1];
            u64 c = qsystem::qubitMask(qubits, inputQubits[0]);
            u64 stride = qsystem::qubitMask(qubits, k);

            for(u64 i = 0; i < in.countRows(); i++){
                u64 mask = i & c;
                u64 smask = i & stride; 
                if(!mask){
                    continue;
                }
                u64 c1_t0 = i & ~smask; //a(*1c*0t*...)
                u64 c1_t1 = i | smask;  //a(*1c*1t*...)

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