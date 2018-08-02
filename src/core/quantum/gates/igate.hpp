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
        /// <Summary>
        /// Virtual destrutor, overwritten by subclasses
        /// </Summary>
        virtual ~igate(){}
        /// <Summary>
        /// Test if a given matrix matches quantum gate specifications (square & unitary)
        /// </Summary>
        static bool isGateable(matrix& a){
            return a.isSquare() && a.isUnitary();
        }
        /// <Summary>
        /// Given a size of a multi-qubit basis, compute the number of qubits
        /// </Summary>
        static bool qubitsForStateSize(size_t statesize){
            size_t counter = 0;
            while(statesize > 1){
                statesize = statesize >> 1;
                counter++;
            }
            return counter;
        }
        //Name of the gate
        virtual std::string name() = 0;
        //Number of qubits it operates on
        virtual size_t inputs() = 0;
        //Using the initial state vector 'in' update the state vector 'out' using the qubit numbers provided in 'inputQubits' list
        virtual void operate(u32 qubits, matrix& in, matrix& out, std::vector<u64> inputQubits) = 0;
};

}
}
}

#endif