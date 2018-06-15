#ifndef _QLIB_QUANTUM_GATES_PAULIZ_H
#define _QLIB_QUANTUM_GATES_PAULIZ_H

#include "onequbitgate.hpp"

namespace qlib {
namespace quantum {
namespace gates {

onequbitgate Z = onequbitgate(
    "Pauli-Z", 
    {
        complex(1), complex(0),
        complex(0), complex(-1)
    });

}
}
}

#endif