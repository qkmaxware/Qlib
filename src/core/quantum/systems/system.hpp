#ifndef _QLIB_QUANTUM_SYSTEM_H
#define _QLIB_QUANTUM_SYSTEM_H

#include "./../../general/object.hpp"
#include "./../../math/complex.hpp" 
#include "./../../math/matrix.hpp" 
#include "./../gates/igate.hpp"

namespace qlib {
namespace quantum {

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
        /// Apply a quantum gate to this system
        /// </Summary>
        virtual void apply(qlib::quantum::gates::igate& gate,std::vector<u64> inputBits) = 0;
        /// <Summary>
        /// Collapse the state and return the value of the measured qubit
        /// </Summary>
        virtual i8 measure(i64 qubit) = 0;
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