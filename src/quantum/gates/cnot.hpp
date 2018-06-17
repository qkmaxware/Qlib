#ifndef _QLIB_QUANTUM_GATES_CNOT_H
#define _QLIB_QUANTUM_GATES_CNOT_H

#include "controlledgate.hpp"
#include "pauliX.hpp"

namespace qlib {
namespace quantum {
namespace gates {

//Controlled NOT, Pauli-X acts as a not gate
controlledgate CNOT = controlledgate(
    "CNOT", 
    X
    );

controlledgate& CX = CNOT;

}
}
}

#endif