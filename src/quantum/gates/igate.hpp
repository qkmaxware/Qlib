#ifndef _QLIB_QUANTUM_IGATE_H
#define _QLIB_QUANTUM_IGATE_H

#include "./../../general/object.hpp"
#include "./../../math/complex.hpp"
#include "./../../math/matrix.hpp"

using namespace qlib::math;

namespace qlib {
namespace quantum {
namespace gates {

//----------------------------------------------------------
// Interface definition
//----------------------------------------------------------
class igate : public xobject {
    private:
    public:
        virtual std::string name() = 0;
        virtual size_t inputs() = 0;
        virtual matrix operate(matrix& state_vector, std::vector<ulong> inputQubits) = 0;
};

}
}
}

#endif