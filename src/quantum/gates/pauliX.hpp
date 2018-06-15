#ifndef _QLIB_QUANTUM_GATES_PAULIX_H
#define _QLIB_QUANTUM_GATES_PAULIX_H

#include "onequbitgate.hpp"

namespace qlib {
namespace quantum {
namespace gates {

onequbitgate X = onequbitgate(
    "Pauli-X", 
    {
        complex(0), complex(1),
        complex(1), complex(0)
    });

}
}
}

#endif