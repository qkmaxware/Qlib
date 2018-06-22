#ifndef _QLIB_QUANTUM_ENSEMBLE_H
#define _QLIB_QUANTUM_ENSEMBLE_H

#include "system.hpp"

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
        /// List of all systems in this collection
        /// </Summary>
        std::vector<qsystem*> set;

    public:
        /// <Summary>
        /// Create an ensemble from a given list of systems
        /// </Summary>
        ensemble(std::vector<qsystem*> eset): set(eset){}
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
        /// Size of the ensemble
        /// </Summary>
        size_t size(){
            return set.size();
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
};

}
}

#endif