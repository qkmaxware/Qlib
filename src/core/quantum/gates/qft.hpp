#ifndef _QLIB_QUANTUM_GATES_QUANTUM_FOURIER_TRANSFORM_H
#define _QLIB_QUANTUM_GATES_QUANTUM_FOURIER_TRANSFORM_H

#include "igate.hpp"
#include <sstream>

namespace qlib {
namespace quantum {
namespace gates {

class quantumFourierTransform : public igate {
    private:
        size_t N;
    public:
        quantumFourierTransform(size_t dims) : N(dims) {}
        //Name of the gate
        std::string name(){
            std::stringstream sb;
            sb << "Quantum Fourier Transform (";
            sb << N;
            sb << " dimentions)";
            return sb.str();
        }
        //Number of qubits it operates on
        size_t inputs(){
            //TODO 
            return N;
        }
        //Using the initial state vector 'in' update the state vector 'out' using the qubit numbers provided in 'inputQubits' list
        void operate(matrix& in, matrix& out, std::vector<u64> inputQubits){
            //TODO
        }
};

}
}
}

#endif