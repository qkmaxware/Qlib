#ifndef _QLIB_QUANTUM_SYSTEM_H
#define _QLIB_QUANTUM_SYSTEM_H

#include "./../../general/object.hpp"
#include "./../../math/complex.hpp" 
#include "./../../math/matrix.hpp" 
#include "./../gates/igate.hpp"

namespace qlib {
namespace quantum {

//TODO QubitIndexOrder order = QubitIndexOrder::LeftToRight on all functions
enum QubitIndexOrder { LeftToRight, RightToLeft };

//----------------------------------------------------------
// Interface definition
//----------------------------------------------------------
/// </Summary>
/// Base class for all quantum systems
/// </Summary>
class qsystem: public xobject {
    private:
    public:
        /// </Summary>
        /// Virtual destructor overwritten by sub-classes
        /// </Summary>
		virtual ~qsystem(){}
        /// </Summary>
        /// Create a bitmask as a selector for a particular qubit in a multi-qubit register
        /// </Summary>
        static u32 qubitMask(u32 qubits, u32 qubit){
            //Sanity check
            if(qubits < 1)
                qubits = 1;
            if(qubit >= qubits)
                qubit = qubits - 1;
            //Work
            u32 topmost = 1 << (qubits - 1);
            return topmost >> qubit;
        }
        /// </Summary>
        /// The number of qubits in the quantum system
        /// </Summary>
        virtual size_t countQubits() = 0;
        /// </Summary>
        /// Apply a quantum gate to this system
        /// </Summary>
        virtual void apply(qlib::quantum::gates::igate& gate,std::vector<u64> inputBits) = 0;
        /// <Summary>
        /// Collapse the state and return the value of the measured qubit
        /// </Summary>
        virtual i8 measure(u32 qubit) = 0;
        /// <Summary>
        /// Set the state of the quantum register
        /// </Summary>
        virtual void setState(std::vector<complex> amp) = 0;
        /// <Summary>
        /// Human readable output
        /// </Summary>
        virtual std::string toString(){
            return xobject::toString();
        };
        /// <Summary>
        /// Prepare qubit in the zero state
        /// </Summary>
        virtual void zero(i64 qubit) = 0;
};

}
}

#endif