#ifndef _QLIB_QUANTUM_QREG_H
#define _QLIB_QUANTUM_QREG_H

#include "./../../general/object.hpp"
#include "./../../math/complex.hpp" 
#include "./../../math/matrix.hpp" 
#include "./../gates/igate.hpp"
#include "system.hpp"

#include <limits>
#include <math.h>
#include <sstream>
#include <vector>

namespace qlib {
namespace quantum {
	
//----------------------------------------------------------
// Class definition
//----------------------------------------------------------

/// <Summary>
/// Represents a register of 'N' qubits
/// </Summary>
class qreg : public qsystem {

    private:
        /// <Summary>
        /// Number of qubits in this register
        /// </Summary>
        u32 qubits;
        /// <Summary>
        /// Number of states represented by the given number of qubits
        /// </Summary>
        u32 states;
        /// <Summary>
        /// State vector of complex amplitudes
        /// </Summary>
        matrix amplitudes;
        /// <Summary>
        /// Random distribution used during measurement
        /// </Summary>
        std::uniform_real_distribution<f32> distribution;

    public:
        /// <Summary>
        /// Quantum register of 1 qubit
        /// </Summary>
        qreg() : qreg(1) {}
        /// <Summary>
        /// Quantum register of 'N' qubits
        /// </Summary>
        qreg(u32 qubits) : amplitudes(1 << qubits,1), distribution(0.0, 1.0){
            this->qubits = qubits;
            this->states = 1 << qubits;
            //initalize to state 0
            amplitudes(0,0) = complex(1);
        }

        /// <Summary>
        /// Number of qubits in this register
        /// </Summary>
        size_t size(){
            return qubits;
        }

        /// <Summary>
        /// Get the complex amplitude matrix
        /// </Summary>
        matrix& state(){
            return this->amplitudes;
        }

        /// </Summary>
        /// Apply a quantum gate to this system
        /// </Summary>
        void apply(qlib::quantum::gates::igate& gate, std::vector<u64> inputBits){
            matrix ref = this->amplitudes;
            gate.operate(ref, state(), inputBits);
        }

        /// <Summary>
        /// Collapse the state and return the value of the measured qubit
        /// </Summary>
        i8 measure(i64 qubit){
            f64 zero = 0.0;
            f64 one = 0.0;

            i64 state = 0;
            i64 mask = 1 << qubit;
            for(std::vector<complex>::iterator it = amplitudes.begin(); it != amplitudes.end(); it++){
                //State has '1'
                if(state && qubit > 0){
                    one += it->sqrMagnitude();
                }
                //State has '0'
                else{
                    zero += it->sqrMagnitude();
                }
                state ++;
            }
            
            f64 prob = this->distribution(rng);
            
            state = 0;
            f64 rootOne = sqrt(one);
            f64 rootZero = sqrt(zero);
            bool zeroChosen = prob <= zero;
            if(zeroChosen){
                //Collapse qubit has chosen to be '0'
                for(std::vector<complex>::iterator it = amplitudes.begin(); it != amplitudes.end(); it++){
                    
                    //State has '1'
                    if(state && qubit > 0){
                        it[state] = complex(0,0);
                    }
                    //State has '0', re-normalize
                    else {
                        it[state] = it[state] / rootZero;
                    }
            
                    state ++;
                }
            }else {
                //Collapse qubit has chosen to be '1'
                for(std::vector<complex>::iterator it = amplitudes.begin(); it != amplitudes.end(); it++){
                    
                    //State has '1', re-normalize
                    if(state && qubit > 0){
                        it[state] = it[state] / rootOne;
                    }
                    //State has '0'
                    else {
                        it[state] = complex(0,0);
                    }
            
                    state ++;
                }
            }

            return zeroChosen ? 0 : 1;
        };

        /// <Summary>
        /// Human readable output
        /// </Summary>
        std::string toString(){
            std::stringstream sb;

            for(uint i = 0; i < states; i++){
                qlib::math::complex& cp = (this->amplitudes(i,0));
                //basically 0, skip
                if(cp.sqrMagnitude() < std::numeric_limits<float>::epsilon()){
                    continue;
                }
                if(i != 0)
                    sb << " + ";
                sb << "(" << cp.toString() << ")|" << i << ">";
            }

            return sb.str();
        }
};
	
}
}

#endif