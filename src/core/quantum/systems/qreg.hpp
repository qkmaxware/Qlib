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
        /// Set the state of the quantum register
        /// </Summary>
        void setState(std::vector<complex> amp){
            f32 sum;
            for(std::vector<complex>::iterator it = amp.begin(); it != amp.end(); it++){
                sum += (*it).sqrMagnitude();
            }
            sum = sqrt(sum);
            for(size_t i = 0; i < amplitudes.countRows(); i++){
                if(i < amp.size()){
                    amplitudes(i,0) = amp[i] / sum;
                }
                else{
                    amplitudes(i,0) = complex(0,0);
                }
            }
        }

        /// </Summary>
        /// The number of qubits in the quantum system
        /// </Summary>
        size_t countQubits(){
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

            u64 mask = 1 << qubit;
            
            for(size_t state = 0; state < amplitudes.countRows(); state++){
                if((state & mask) > 0){
                    //State has '1' at qubit
                    one += amplitudes(state, 0).sqrMagnitude();
                }
                else{
                    //State has '0' at qubit
                    zero += amplitudes(state, 0).sqrMagnitude();
                }
            }
            
            f64 prob = this->distribution(rng);
            
            f64 rootOne = sqrt(one);
            f64 rootZero = sqrt(zero);
            bool zeroChosen = prob <= zero;
            if(zeroChosen){
                //Collapse qubit has chosen to be '0'
                for(size_t r = 0; r < amplitudes.countRows(); r++){
                    //State has '1'
                    if((r & mask) > 0){
                        amplitudes(r,0) = complex(0,0);
                    }
                    //State has '0', re-normalize
                    else {
                        amplitudes(r,0) = amplitudes(r,0) / rootZero;
                    }
                }
            }else {
                //Collapse qubit has chosen to be '1'
                for(size_t r = 0; r < amplitudes.countRows(); r++){
                    //State has '1', re-normalize
                    if((r & mask) > 0){
                       amplitudes(r,0) = amplitudes(r,0) / rootOne;
                    }
                    //State has '0'
                    else {
                        amplitudes(r,0) = complex(0,0);
                    }
                }
            }

            return zeroChosen ? 0 : 1;
        };

        /// <Summary>
        /// Prepare qubit in the zero state
        /// </Summary>
        virtual void zero(i64 qubit) {
            f64 zero = 0.0;
            u64 mask = 1 << qubit;
            //Sum all states where qubit is zero
            for(size_t state = 0; state < amplitudes.countRows(); state++){
                if ((state & mask) < 1){
                    zero += amplitudes(state, 0).sqrMagnitude();
                }
            }

            //Renormalize, set states where qubit is 1 to 0
            f64 rootZero = sqrt(zero);
            for(size_t r = 0; r < amplitudes.countRows(); r++){
                //State has '1'
                if((r & mask) > 0){
                    amplitudes(r,0) = complex(0,0);
                }
                //State has '0', re-normalize
                else {
                    amplitudes(r,0) = amplitudes(r,0) / rootZero;
                }
            }
        }

        /// <Summary>
        /// Human readable output
        /// </Summary>
        std::string toString(){
            std::stringstream sb;
            bool first = true;

            for(u32 i = 0; i < states; i++){
                qlib::math::complex& cp = (this->amplitudes(i,0));
                //basically 0, skip
                if(cp.sqrMagnitude() < std::numeric_limits<float>::epsilon()){
                    continue;
                }
                if(!first)
                    sb << " + ";
                sb << "(" << cp.toString() << ")|" << i << ">";
                first = false;
            }

            return sb.str();
        }
};
	
}
}

#endif