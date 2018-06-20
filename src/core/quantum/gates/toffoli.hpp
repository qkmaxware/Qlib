#ifndef _QLIB_QUANTUM_GATES_TOFFOLI_H
#define _QLIB_QUANTUM_GATES_TOFFOLI_H

#include "controlled2gate.hpp"
#include "pauliX.hpp"

namespace qlib {
namespace quantum {
namespace gates {

//Controlled NOT, Pauli-X acts as a not gate
controlled2gate CCNOT = controlled2gate(
    "CCNOT", 
    X
    );

controlled2gate& TOFFOLI = CCNOT;

}
}
}

#endif