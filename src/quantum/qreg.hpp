#ifndef _QLIB_QUANTUM_QREG_H
#define _QLIB_QUANTUM_QREG_H

#include "./../general/object.hpp"
#include "./../math/complex.hpp" 
#include "./../math/matrix.hpp" 

#include <sstream>
#include <vector>

namespace qlib {
namespace quantum {
	
//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class qreg : public xobject {

    private:
        uint qubits;
        uint states;
        std::vector<qlib::math::complex> amplitudes;

    public:
        qreg(uint qubits){
            if(qubits < 1)
                qubits = 1;
            this->qubits = qubits;

            this->states = 1 << qubits;

            this->amplitudes = std::vector<qlib::math::complex>(this->states);
            for(uint i = 0; i < this->states; i++){
                this->amplitudes.push_back(qlib::math::complex());
            }
        }

        std::string toString(){
            std::stringstream sb;

            for(uint i = 0; i < states; i++){
                if(i != 0)
                    sb << " + ";
                sb << "(" << (this->amplitudes[i]).toString() << ")|" << i << ">";
            }

            return sb.str();
        }
};
	
}
}

#endif