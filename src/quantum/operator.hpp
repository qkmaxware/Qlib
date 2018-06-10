#ifndef _QLIB_QUANTUM_OPERATOR_H
#define _QLIB_QUANTUM_OPERATOR_H

#include <math.h>
#include <vector>

#include "./../general/object.hpp"
#include "./../math/matrix.hpp"

using namespace qlib::math;

namespace qlib {
namespace quantum {

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class qop : public xobject{

    private:
        std::string name;
        uint states;
        matrix m;

    public:
        /// <Summary>
        /// Create a new quanutm operator for the given number of states
        /// </Summary>
        qop(std::string name, int states, std::vector<complex> values) : m(1,1){
            this->name = name;

            if(states < 1)
                states = 1;
            this->states = states;

            int matsize = states * 2;
            this->m = matrix(matsize, matsize, values);
        }

        /// <Summary>
        /// Pretty print
        /// </Summary>
        std::string toString(){
            std::stringstream sb;

            sb << (this->name) << "(" << (this->states) << ") -" << (this->m.toString());

            return sb.str();
        }
};

//----------------------------------------------------------
// Standard operators
//----------------------------------------------------------

qop H = qop("Hadamard", 1, {
        complex(1.0f/sqrt(2.0f)), complex(1.0f/sqrt(2.0f)),
        complex(1.0f/sqrt(2.0f)), complex(-1.0f/sqrt(2.0f))
    });

}
}

#endif