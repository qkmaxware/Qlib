#ifndef _QLIB_QUANTUM_GATES_HADAMARD_H
#define _QLIB_QUANTUM_GATES_HADAMARD_H

#include "onequbitgate.hpp"
#include "controlledgate.hpp"

namespace qlib {
namespace quantum {
namespace gates {

onequbitgate H = onequbitgate(
    "Hadamard", 
    {
        complex(1.0f/sqrt(2.0f)), complex(1.0f/sqrt(2.0f)),
        complex(1.0f/sqrt(2.0f)), complex(-1.0f/sqrt(2.0f))
    });

controlledgate CH = controlledgate(
    "Controlled Hadamard",
    H
);

}
}
}

#endif