/// </Summary>
/// Standard included gates
/// </Summary>

#ifndef _QLIB_QUANTUM_STANDARD_GATES_H
#define _QLIB_QUANTUM_STANDARD_GATES_H

#include "./gates/ident.hpp"
#include "./gates/hadamard.hpp"
#include "./gates/pauliX.hpp"
#include "./gates/pauliY.hpp"
#include "./gates/pauliZ.hpp"
#include "./gates/phase.hpp"

#include "./gates/cnot.hpp"
#include "./gates/cy.hpp"
#include "./gates/cz.hpp"

#include "./gates/toffoli.hpp"

#include "./gates/qft.hpp"

/*
qop SWAP = qop("Swap", 2, {
        complex(1), complex(0), complex(0), complex(0),
        complex(0), complex(0), complex(1), complex(0),
        complex(0), complex(1), complex(0), complex(0),
        complex(0), complex(0), complex(0), complex(1)
    });
*/

#endif