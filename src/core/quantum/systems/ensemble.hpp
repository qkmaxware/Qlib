#ifndef _QLIB_QUANTUM_ENSEMBLE_H
#define _QLIB_QUANTUM_ENSEMBLE_H

#include "system.hpp"
#include "qreg.hpp"

#include <vector>
#include <map>

namespace qlib {
namespace quantum {

/// <Summary>
/// Represents a collection of multiple identical quantum systems
/// </Summary>
class ensemble : public qsystem{
    private:
        /// <Summary>
        /// List of all systems in this ensemble
        /// </Summary>
        std::vector<qsystem*> set;
        /// <Summary>
        /// Number of qubits used for each system in the ensemble
        /// </Summary>
        u32 qubits;

    public:
        /// <Summary>
        /// Create an ensemble from a given list of systems
        /// </Summary>
        ensemble(u32 qubits, size_t systems): set(), qubits(qubits){
            for(size_t t = 0; t < systems; t++){
                set.push_back(new qreg(qubits));
            }
        }
        /// <Summary>
        /// Delete the ensemble and cleanup pointers
        /// </Summary>
        ~ensemble(){
            //Clean up memory
            for(std::vector<qsystem*>::iterator it = set.begin(); it != set.end(); it++) {
                delete *it;
            }
        }

        /// <Summary>
        /// Set the state of the quantum register
        /// </Summary>
        void setState(std::vector<complex> amp){
            for(std::vector<qsystem*>::iterator it = set.begin(); it != set.end(); it++) {
                (*it)->setState(amp);
            }
        }

        /// <Summary>
        /// Size of the ensemble
        /// </Summary>
        size_t size(){
            return set.size();
        }

        /// </Summary>
        /// The number of qubits in the quantum system
        /// </Summary>
        size_t countQubits(){
            return qubits;
        }

        /// <Summary>
        /// Apply operator to all systems in the ensemble
        /// </Summary>
        void apply(qlib::quantum::gates::igate& op,std::vector<u64> inputBits){
            for(std::vector<qsystem*>::iterator it = set.begin(); it != set.end(); it++) {
                (*it)->apply(op, inputBits);
            }
        }

        /// <Summary>
        /// Measure all systems in the ensemble and return most common answer
        /// </Summary>
        i8 measure(i64 qubit){
            std::map<i8, u64> map;
            u64 max = 0;
            for(std::vector<qsystem*>::iterator it = set.begin(); it != set.end(); it++){
                i8 r = (*it)->measure(qubit);
                u64 count = ++map[r];
                if(count > map[max]){
                    max = r;
                }
            }
            return max;
        }

        /// <Summary>
        /// Prepare qubit in the zero state
        /// </Summary>
        virtual void zero(i64 qubit) {
            for(std::vector<qsystem*>::iterator it = set.begin(); it != set.end(); it++){
                (*it)->zero(qubit);
            }
        }
};

}
}

#endif