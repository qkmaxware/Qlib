#ifndef _QLIB_QUANTUM_GATES_QUANTUM_FOURIER_TRANSFORM_H
#define _QLIB_QUANTUM_GATES_QUANTUM_FOURIER_TRANSFORM_H

#include "igate.hpp"
#include "./../../general/constants.h"
#include <math.h>

namespace qlib {
namespace quantum {
namespace gates {

class quantumFourierTransform : public igate {
    private:
        complex wn(size_t N) {
            //e^(ix) = cos(x) + isin(x)
            //e^(2 pi i / N)
            f64 x = 2 * M_PI * (1.0 / N);
            f64 real = cos(x);
            f64 img = sin(x);
            return complex(real, img);
        }
        complex wnPow(size_t N, size_t j){
            complex _wn = wn(N);
            for(size_t k = 0; k < j; k++){
                _wn = _wn * _wn; 
            }
            return _wn;
        }

    public:
        //Name of the gate
        std::string name(){
            return "Quantum Fourier Transform";
        }
        //Number of qubits it operates on
        size_t inputs(){
            //TODO 
            return 0;
        }
        //Using the initial state vector 'in' update the state vector 'out' using the qubit numbers provided in 'inputQubits' list
        void operate(matrix& in, matrix& out, std::vector<u64> inputQubits){
            //TODO
            f64 one_over_sqrt_N = 1.0 / sqrt(in.countRows());
            for(size_t k = 0; k < in.countRows(); k++){
                complex sum;
                for(size_t j = 0; j < in.countRows(); j++){
                    sum = sum + in(j,0) * wnPow(in.countRows(), j * k );
                }
                out(k,0) = one_over_sqrt_N * sum;
            }
        }
};

}
}
}

#endif