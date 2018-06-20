#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <iostream>
#include <vector>
#include <map>

#include "./../core/qlib.h"

typedef std::vector<bool> creg;

using namespace std;
using namespace qlib::quantum;

namespace runtime {

class environment {
    private:
        map<string,qreg> quantum_registers;
        map<string,creg> classic_registers;

    public:
        void getGate(string name, qlib::quantum::gates::igate* gateptr){
            //*gateptr = ...;
        }

        creg& getCreg(string name){
            return classic_registers[name];
        }

        void setCreg(string name, ulong size){
            creg r;
            for(ulong i = 0; i < size; i++){
                r.push_back(false);
            }
            classic_registers[name] = r;
        }

        qreg& getQreg(string name){
            return quantum_registers[name];
        }

        void setQreg(string name, ulong size){
            qreg q(size);
            quantum_registers[name] = q;
        }
};

}

#endif