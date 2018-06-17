#ifndef _QLIB_QUANTUM_STANDARD_GATES_H
#define _QLIB_QUANTUM_STANDARD_GATES_H

#include "./gates/hadamard.hpp"
#include "./gates/pauliX.hpp"
#include "./gates/pauliY.hpp"
#include "./gates/pauliZ.hpp"
#include "./gates/cnot.hpp"

//TODO phase shift gate
/*
qop SWAP = qop("Swap", 2, {
        complex(1), complex(0), complex(0), complex(0),
        complex(0), complex(0), complex(1), complex(0),
        complex(0), complex(1), complex(0), complex(0),
        complex(0), complex(0), complex(0), complex(1)
    });

qop CNOT = qop("Controlled Not", 2, {
        complex(1), complex(0), complex(0), complex(0),
        complex(0), complex(1), complex(0), complex(0),
        complex(0), complex(0), complex(0), complex(1),
        complex(0), complex(0), complex(1), complex(0)
    });
*/

#endif