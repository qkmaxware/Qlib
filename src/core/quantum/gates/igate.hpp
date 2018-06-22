#ifndef _QLIB_QUANTUM_IGATE_H
#define _QLIB_QUANTUM_IGATE_H

#include "./../../general/object.hpp"
#include "./../../math/complex.hpp"
#include "./../../math/matrix.hpp"

using namespace qlib::math;

namespace qlib {
namespace quantum {
namespace gates {

//----------------------------------------------------------
// Interface definition
//----------------------------------------------------------
class igate : public xobject {
    private:
    public:
        //Name of the gate
        virtual std::string name() = 0;
        //Number of qubits it operates on
        virtual size_t inputs() = 0;
        //Using the initial state vector 'in' update the state vector 'out' using the qubit numbers provided in 'inputQubits' list
        virtual void operate(matrix& in, matrix& out, std::vector<u64> inputQubits) = 0;
};

}
}
}

#endif