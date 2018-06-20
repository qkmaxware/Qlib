#ifndef _QLIB_QUANTUM_GATES_PHASE_H
#define _QLIB_QUANTUM_GATES_PHASE_H

#include "onequbitgate.hpp"
#include <math.h>

namespace qlib {
namespace quantum {
namespace gates {

onequbitgate phase(f32 angle){
    std::stringstream sb;
    sb << "Phase " << angle;
    
    return onequbitgate(
        sb.str(), 
        {
            complex(1), complex(0),
            complex(0), complex(cos(angle), sin(angle))
        });
}

}
}
}

#endif