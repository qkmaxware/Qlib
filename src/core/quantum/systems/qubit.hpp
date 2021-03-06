#ifndef _QLIB_QUANTUM_QBIT_H
#define _QLIB_QUANTUM_QBIT_H

#include "./../../general/object.hpp"
#include "./../../math/complex.hpp" 
#include "./../../math/matrix.hpp" 
#include "./../gates/igate.hpp"
#include "system.hpp"

#include <math.h> 
#include <sstream>
#include <vector>

namespace qlib {
namespace quantum {

/// <Summary>
/// Representation of a single qubit 
/// </Summary>
class qubit : public qsystem {

    private:
        /// <Summary>
        /// State vector amplitudes
        /// </Summary>
        qlib::math::matrix vec;
        /// <Summary>
        /// Random distribution used during measurement
        /// </Summary>
        std::uniform_real_distribution<f32> distribution;

    public:
        /// <Summary>
        /// Create a qubit in the |0> state
        /// </Summary>
        qubit() : vec(2,1), distribution(0.0, 1.0){
            vec(0,0) = qlib::math::complex(1,0);
        }

        /// <Summary>
        /// Create a qubit in desired superposition
        /// </Summary>
        qubit(qlib::math::complex zero, qlib::math::complex one): vec(2,1) {
            //Ensure normalization condition
            f32 mag = sqrt(zero.sqrMagnitude() + one.sqrMagnitude());
            vec(0,0) = zero/mag;
            vec(1,0) = one/mag;
        }

        /// <Summary>
        /// Set the state of the quantum register
        /// </Summary>
        void setState(std::vector<complex> amp){
            if(amp.size() >= 1){
                vec(0,0) = amp[0];
            }else{
                vec(0,0) = qlib::math::complex(0,0);
            }
            if(amp.size() >= 2){
                vec(1,0) = amp[1];
            }else{
                vec(0,0) = qlib::math::complex(0,0);
            }
            
            f32 mag = sqrt(vec(0,0).sqrMagnitude() + vec(1,0).sqrMagnitude());
            vec(0,0) = vec(0,0)/mag;
            vec(1,0) = vec(1,0)/mag;
        }

        /// <Summary>
        /// Current state column vector
        /// </Summary>
        matrix& state(){
            return this->vec;
        }

        /// </Summary>
        /// The number of qubits in the quantum system
        /// </Summary>
        size_t countQubits(){
            return 1;
        }

        /// <Summary>
        /// Apply a quantum gate to this qubit
        /// </Summary>
        void apply(qlib::quantum::gates::igate& gate, std::vector<u64> inputBits){
            if(gate.inputs() != 1){
                throw std::length_error("Only one qubit operators can be applied to single qubit systems"); 
            }
            matrix ref = this->vec;
            gate.operate(1, vec, state(), inputBits);
        }

        /// <Summary>
        /// Apply a quantum measurement to this qubit
        /// </Summary>
        i8 measure(u32 qubit = 0){ 
            //Prob of |0> is vec(0,0).sqrMagnitude();
            f32 p0 = vec(0,0).sqrMagnitude();
            //Prob of |1> is vec(1,0).sqrMagnitude() or 1-p0 due to normalization
            //f32 p1 = 1.0 - p0;

            f32 r = this->distribution(rng);

            //System collapse dictated by |new> = normalize(Pi |old>)
            //P0 = |0><0|
            //P1 = |1><1|
            if(r <= p0){
                this->vec = matrix(2,1, {1, 0});
                return 0;
            }
            else{
                this->vec = matrix(2,1, {0, 1});
                return 1;
            }
        }

        /// <Summary>
        /// Prepare qubit in the zero state
        /// </Summary>
        virtual void zero(i64 qubit) {
            this->vec = matrix(2,1, {1, 0});
        }

        /// <Summary>
        /// Pretty print
        /// </Summary>
        std::string toString(){
            std::stringstream sb;

            sb << (this->vec(0,0).toString()) << "|0> + " << (this->vec(1,0).toString()) << "|1>";

            return sb.str();
        }

};


}
}

#endif