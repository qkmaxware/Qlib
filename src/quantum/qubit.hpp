#ifndef _QLIB_QUANTUM_QBIT_H
#define _QLIB_QUANTUM_QBIT_H

#include "./../general/object.hpp"
#include "./../math/complex.hpp" 
#include "./../math/matrix.hpp" 

#include <sstream>
#include <vector>

namespace qlib {
namespace quantum {

class qubit : public xobject {

    private:
        //Component for |0>
        qlib::math::complex a;
        //Component fot |1>
        qlib::math::complex b;
    
    public:
        /// <Summary>
        /// Create a qubit in the |0> state
        /// </Summary>
        qubit(){
            this->a = qlib::math::complex(1,0);
            this->b = qlib::math::complex(0,0);
        }

        /// <Summary>
        /// Create a qubit in desired superposition
        /// </Summary>
        qubit(qlib::math::complex zero, qlib::math::complex one){
            this->a = zero;
            this->b = one;
        }

        /// <Summary>
        /// Pretty print
        /// </Summary>
        std::string toString(){
            std::stringstream sb;

            sb << (this->a.toString()) << "|0> + " << (this->b.toString()) << "|1>";

            return sb.str();
        }

};


}
}

#endif