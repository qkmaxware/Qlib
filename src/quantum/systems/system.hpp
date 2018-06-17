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
class qsystem: public xobject {
    private:
    public:
        //Apply a quantum gate to this system
        virtual void apply(qlib::quantum::gates::igate& gate,std::vector<ulong> inputBits) = 0;
        //Collapse the state and return the value of the measured qubit
        virtual i32 measure(i32 qubit) = 0;
        //Human readable output
        virtual std::string toString(){
            return xobject::toString();
        };
};

}
}

#endif