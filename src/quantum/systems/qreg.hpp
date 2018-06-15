#ifndef _QLIB_QUANTUM_QREG_H
#define _QLIB_QUANTUM_QREG_H

#include "./../../general/object.hpp"
#include "./../../math/complex.hpp" 
#include "./../../math/matrix.hpp" 
#include "./../gates/igate.hpp"
#include "system.hpp"

#include <sstream>
#include <vector>

namespace qlib {
namespace quantum {
	
//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class qreg : public qsystem {

    private:
        u32 qubits;
        u32 states;
        matrix amplitudes;

    public:
        qreg(u32 qubits) : amplitudes(1 << qubits,1){
            this->qubits = qubits;

            this->states = 1 << qubits;
        }

        matrix& state(){
            return this->amplitudes;
        }

        void apply(qlib::quantum::gates::igate& gate, args<ulong> inputBits){

        }

        i32 measure(i32 qubit){
            return 0;
        };

        std::string toString(){
            std::stringstream sb;

            for(uint i = 0; i < states; i++){
                if(i != 0)
                    sb << " + ";
                sb << "(" << (this->amplitudes(i,1)).toString() << ")|" << i << ">";
            }

            return sb.str();
        }
};
	
}
}

#endif