#ifndef _QLIB_QUANTUM_GATES_CY_H
#define _QLIB_QUANTUM_GATES_CY_H

#include "controlledgate.hpp"
#include "pauliY.hpp"

namespace qlib {
namespace quantum {
namespace gates {

controlledgate CY = controlledgate(
    "CY", 
    Y
    );

}
}
}

#endif