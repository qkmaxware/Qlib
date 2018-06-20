#ifndef _QLIB_QUANTUM_GATES_CZ_H
#define _QLIB_QUANTUM_GATES_CZ_H

#include "controlledgate.hpp"
#include "pauliZ.hpp"

namespace qlib {
namespace quantum {
namespace gates {

controlledgate CZ = controlledgate(
    "CZ", 
    Z
    );

}
}
}

#endif