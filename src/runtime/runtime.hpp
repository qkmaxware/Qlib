#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <sstream>
#include <iostream>
#include <vector>
#include <map>

#include "./../core/qlib.h"

typedef std::vector<bool> creg;

using namespace std;
using namespace qlib::quantum;
using namespace qlib::quantum::gates;

namespace qasm {

namespace runtime {

class runtimeexception : public std::exception {
    private:
        std::string what_message;

    public:
        std::string msg;

        runtimeexception(std::string message) : what_message(message){
        }

        const char* what() const throw() {
            return what_message.c_str();
        }
};

class environment {
    private:
        map<string,qreg> quantum_registers;
        map<string,creg> classic_registers;

    public:
        void getGate(string name, igate* gateptr){
            //*gateptr = ...;
        }

        creg& getCreg(string name){
            return classic_registers[name];
        }

        bool hasCreg(string name){
            return classic_registers.count(name) > 0;
        }

        void setCreg(string name, u64 size){
            creg r;
            for(ulong i = 0; i < size; i++){
                r.push_back(false);
            }
            classic_registers[name] = r;
        }

        qreg& getQreg(string name){
            return quantum_registers[name];
        }

        bool hasQreg(string name){
            return quantum_registers.count(name) > 0;
        }

        void setQreg(string name, u64 size){
            qreg q(size);
            quantum_registers[name] = q;
        }
};

}


namespace exec {

    class executable {
        private:
        public: 
            virtual void invoke_rootprogram(runtime::environment& env) {};
            virtual void invoke_subprogram(runtime::environment& env) {};
    };

    class apply_subcircuit : public executable {
        public: 
            std::string name;
            std::vector<std::string> params;

            apply_subcircuit(string name) : name(name) {}
    };

    class apply_gate : public executable {
        public:
            std::string name;
            std::vector<std::string> param_names;
            std::vector<u64> param_indecies;

            apply_gate(string name): name(name){}

            virtual void invoke_rootprogram(runtime::environment& env) {
                igate* ptr = NULL;
                env.getGate(name, ptr);

                //Gate doesn't exist
                if(ptr == NULL){
                    stringstream sb;
                    sb << "No gate: ";
                    sb << name;
                    sb << " exists";
                    throw qasm::runtime::runtimeexception(sb.str());
                }
                
                //names and indeces doen't match
                if(param_names.size() != param_indecies.size()){
                    stringstream sb;
                    sb << "Number of parameter names and indeces do not match";
                    throw qasm::runtime::runtimeexception(sb.str());
                }

                //Confirm same name
                std::string name;
                bool first = true;
                for(vector<string>::iterator it = param_names.begin(); it != param_names.end(); it++){
                    if(first){
                        first = false;
                        name = *it;
                        continue;
                    }
                    
                    if(*it != name){
                        stringstream sb;
                        sb << "Expecting gate to operate on register ";
                        sb << name;
                        sb << " but got ";
                        sb << (*it);
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                }

                if(env.hasQreg(name)){
                    env.getQreg(name).apply(*ptr, param_indecies);
                }else{
                    //Register doesn't exist
                    stringstream sb;
                    sb << "No quantum register: ";
                    sb << name;
                    sb << " exists";
                    throw qasm::runtime::runtimeexception(sb.str());
                }

            };
    };

    class print : public executable{
        public:
            std::string reference;

            print(string ref): reference(ref){}

            virtual void invoke_rootprogram(runtime::environment& env) {
                std::string msg;
                std::cout << "print: ";
                if(env.hasCreg(reference)){
                    creg& r = env.getCreg(reference);
                    for (creg::const_iterator i = r.begin(); i != r.end(); i++)
                        std::cout << (*i == true ? "1" : "0");
                }else if(env.hasQreg(reference)){
                    std::cout << env.getQreg(reference).toString();
                }else{
                    //Throw exception
                    stringstream sb;
                    sb << "No register: ";
                    sb << reference;
                    sb << " to print";
                    throw qasm::runtime::runtimeexception(sb.str());
                }
                std::cout << std::endl;
            };
    };

    class measurement : public executable {
        public: 
            std::string qreg;
            long qindex;
            std::string creg;
            long cindex;
            bool measureWhole;

            measurement(string qr, long qi, string cr, long ci) : qreg(qr), qindex(qi), creg(cr), cindex(ci), measureWhole(false) {}
            measurement(string qr, string cr) : qreg(qr), qindex(-1), creg(cr), cindex(-1), measureWhole(true) {}
    
            virtual void invoke_rootprogram(runtime::environment& env) {
                if(!env.hasQreg(qreg) || !env.hasCreg(creg)){
                    stringstream sb;
                    sb << "Registers: ";
                    sb << qreg << ", " << creg << " don't correspond to a quantum and classical register pair";
                    throw qasm::runtime::runtimeexception(sb.str());
                }
                qlib::quantum::qreg& q = env.getQreg(qreg);
                std::vector<bool>& c = env.getCreg(creg);

                if(measureWhole){
                    for(size_t s = 0; s < q.size(); s++){
                        u8 value = q.measure(s);
                        if(s < c.size())
                            c[s] = value;
                    }
                }else {
                    
                    if(qindex < 0 || qindex >= q.size()){
                        //Qbit index out of range
                        stringstream sb;
                        sb << "Quantum index: ";
                        sb << qreg << qindex << " out of range";
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                    if(cindex < 0 || cindex >= c.size()){
                        //Cbit index out of range
                        //Qbit index out of range
                        stringstream sb;
                        sb << "Classical index: ";
                        sb << creg << cindex << " out of range";
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                    u8 value = q.measure(qindex);
                    c[cindex] = value;
                }
            };
    };

    class declaration : public executable{
        public: 
            std::string type;
            std::string name;
            u64 size;
        
            declaration(string t, string n, u64 s) : type(t), name(n), size(s) {}

            virtual void invoke_rootprogram(runtime::environment& env) {
                if(type == "qreg"){
                    env.setQreg(name, size);
                }else if(type == "creg"){
                    env.setCreg(name, size);
                }else{
                    stringstream sb;
                    sb << "Unknown type: ";
                    sb << type;
                    sb << " can't define variable ";
                    sb << name; 
                    sb << " of size ";
                    sb << size;
                    throw qasm::runtime::runtimeexception(sb.str());
                }
            };
    };

}
}

#endif