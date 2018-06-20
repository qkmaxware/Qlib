#ifndef _QLIB_QUANTUM_GATES_IDENT_H
#define _QLIB_QUANTUM_GATES_IDENT_H

#include "onequbitgate.hpp"

namespace qlib {
namespace quantum {
namespace gates {

onequbitgate I = onequbitgate(
    "Identity", 
    {
        complex(1), complex(0),
        complex(0), complex(1)
    });

}
}
}

#endif