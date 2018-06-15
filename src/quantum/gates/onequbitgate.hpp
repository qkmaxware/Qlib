#ifndef _QLIB_QUANTUM_ONEQUBITGATE_H
#define _QLIB_QUANTUM_ONEQUBITGATE_H

#include "./igate.hpp"


namespace qlib {
namespace quantum {
namespace gates {

//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

class onequbitgate : public igate {

    private:
        std::string sname;
        matrix m;

    public:
        /// <Summary>
        /// Create a new quanutm operator for the given number of states
        /// </Summary>
        onequbitgate(std::string name, std::vector<complex> values) : m(2, 2, values){
            this->sname = name;
        }

        /// <Summary>
        /// Number of input qubits this operator can be applied to
        /// </Summary>
        size_t inputs(){
            return 1;
        } 

        /// <Summary>
        /// The name of this gate
        /// </Summary>
        std::string name(){
            return this->sname;
        }

        /// <Summary>
        /// Operate on a given column vector state superposition
        /// </Summary>
        matrix operate(matrix& state_vector, args<ulong> inputQubits){
            //matrix r(state_vector.countRows(), state_vector.countColumns());
            //Copy from state_vector
            //matrix pair(2,1, {state_vector(), state_vector()});
            //matrix pr = (this->m) * pair;
            //Copy to r;
            //return r;
            return (this->m) * state_vector;
        }

        /// <Summary>
        /// Returns the matrix representation of this operator
        /// </Summary>
        qlib::math::matrix& getMatrix(){
            return this->m;
        }

        /// <Summary>
        /// Pretty print
        /// </Summary>
        std::string toString(){
            std::stringstream sb;

            sb << (this->sname) << "(" << (1) << ") -" << (this->m.toString());

            return sb.str();
        }
};

}
}
}

#endif