#ifndef _QLIB_QUANTUM_ENSEMBLE_H
#define _QLIB_QUANTUM_ENSEMBLE_H

#include "system.hpp"

#include <vector>
#include <map>

namespace qlib {
namespace quantum {

class ensemble {
    private:
        std::vector<qsystem*> set;

    public:
        ensemble(std::vector<qsystem*> eset): set(eset){}
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
        void apply(qlib::quantum::gates::igate& op,std::vector<ulong> inputBits){
            for(std::vector<qsystem*>::iterator it = set.begin(); it != set.end(); it++) {
                (*it)->apply(op, inputBits);
            }
        }

        /// <Summary>
        /// Measure all systems in the ensemble and return most common answer
        /// </Summary>
        i32 measure(i32 qubit){
            std::map<i32, u32> map;
            u32 max = 0;
            for(std::vector<qsystem*>::iterator it = set.begin(); it != set.end(); it++){
                i32 r = (*it)->measure(qubit);
                u32 count = ++map[r];
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