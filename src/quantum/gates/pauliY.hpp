#ifndef _QLIB_QUANTUM_GATES_PAULIY_H
#define _QLIB_QUANTUM_GATES_PAULIY_H

#include "onequbitgate.hpp"

namespace qlib {
namespace quantum {
namespace gates {

onequbitgate Y = onequbitgate(
    "Pauli-Y", 
    {
        complex(0,0), complex(0,-1),
        complex(0,1), complex(0,0)
    });

}
}
}

#endif