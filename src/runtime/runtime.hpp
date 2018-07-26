#ifndef _RUNTIME_H
#define _RUNTIME_H

#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include "./../core/qlib.h"

typedef std::vector<bool> creg;

using namespace std;
using namespace qlib::quantum;
using namespace qlib::quantum::gates;

namespace qasm {

namespace runtime {

/// <Summary>
/// Class for exceptions thrown when executing instructions
/// </Summary>
class runtimeexception : public std::exception {
    private:
        /// <Summary>
        /// Exception message
        /// </Summary>
        std::string what_message;

    public:

        /// <Summary>
        /// Create exception
        /// </Summary>
        runtimeexception(std::string message) : what_message(message){
        }

        /// <Summary>
        /// Get cause message
        /// </Summary>
        const char* what() const throw() {
            return what_message.c_str();
        }
};

/// <Summary>
/// Represents a runtime environment including variable values
/// </Summary>
class environment {
    private:
        /// <Summary>
        /// List of assigned quantum variables
        /// </Summary>
        map<string,qreg> quantum_registers;
        /// <Summary>
        /// List of assigned classic varaibles
        /// </Summary>
        map<string,creg> classic_registers;
        /// <Summary>
        /// List of stored gates
        /// </Summary>
        map<string,igate*> registered_gates;
        /// <Summary>
        /// List of user defined gates that need to be cleaned up when the environment is cleared
        /// </Summary>
        vector<igate*> gates_to_cleanup;
    public:
        /// <Summary>
        /// Create a new runtime environment with the default definitions
        /// </Summary>
        environment() : quantum_registers(), classic_registers(), registered_gates(), gates_to_cleanup(){
            registered_gates["h"] = &qlib::quantum::gates::H;
            registered_gates["i"] = &qlib::quantum::gates::I;
            registered_gates["x"] = &qlib::quantum::gates::X;
            registered_gates["not"] = &qlib::quantum::gates::X;
            registered_gates["y"] = &qlib::quantum::gates::Y;
            registered_gates["z"] = &qlib::quantum::gates::Z;

            registered_gates["ch"] = &qlib::quantum::gates::CH;
            registered_gates["cx"] = &qlib::quantum::gates::CX;
            registered_gates["cnot"] = &qlib::quantum::gates::CNOT;
            registered_gates["cy"] = &qlib::quantum::gates::CY;
            registered_gates["cz"] = &qlib::quantum::gates::CZ;

            registered_gates["toffoli"] = &qlib::quantum::gates::TOFFOLI;
            registered_gates["ccnot"] = &qlib::quantum::gates::CCNOT;
        }

        /// <Summary>
        /// Destroy environment and all definitions
        /// </Summary>
        ~environment(){
            for(vector<igate*>::iterator it = gates_to_cleanup.begin(); it != gates_to_cleanup.end(); it++){
                delete *it;
            }
        }

        /// <Summary>
        /// For a given name, return the associated gate object
        /// </Summary>
        igate* getGate(string name){
            bool hasGate = registered_gates.count(name) > 0;
            if(hasGate){
                return registered_gates[name];
            }
            else{
                stringstream sb;
                sb << "Gate \"" << name << "\" is not defined.";
                throw runtimeexception(sb.str());
            }
        }

        /// <Summary>
        /// Add a user defined gate to the environment. Gate is cleaned-up when the environment is cleaned-up.
        /// </Summary>
        void buildGate(string name, complex aa, complex ab, complex ba, complex bb){
            onequbitgate* mygate = new onequbitgate(name, {aa, ab, ba, bb});
            registered_gates[name] = mygate;
            //TODO build controlled U gate too?
            //controlledgate controlledMygate = controlledgate("Controlled - U", *mygate);
            gates_to_cleanup.push_back(mygate); 
        }

        /// <Summary>
        /// Get classic variable from a name
        /// </Summary>
        creg& getCreg(string name){
            return classic_registers[name];
        }

        /// <Summary>
        /// Test if a classic variable with name exists
        /// </Summary>
        bool hasCreg(string name){
            return classic_registers.count(name) > 0;
        }

        /// <Summary>
        /// Bind a classic variable to a name
        /// </Summary>
        void setCreg(string name, u64 size){
            creg r;
            for(u64 i = 0; i < size; i++){
                r.push_back(false);
            }
            classic_registers[name] = r;
        }

        /// <Summary>
        /// Get quantum variable from a name
        /// </Summary>
        qreg& getQreg(string name){
            return quantum_registers[name];
        }

        /// <Summary>
        /// Test if a quantum variable with name exists
        /// </Summary>
        bool hasQreg(string name){
            return quantum_registers.count(name) > 0;
        }

        /// <Summary>
        /// Bind a quantum variable to a name
        /// </Summary>
        void setQreg(string name, u64 size){
            qreg q(size);
            quantum_registers[name] = q;
        }
};

}


namespace exec {

    /// <Summary>
    /// Represents an instruction that can be run
    /// </Summary>
    class executable {
        private:
        public: 
			virtual ~executable(){}
            /// <Summary>
            /// Run instruction's primary operation
            /// </Summary>
            virtual void invoke_rootprogram(runtime::environment& env) {};
            /// <Summary>
            /// Run instruction's secondary operation
            /// </Summary>
            virtual void invoke_subprogram(runtime::environment& env) {};
    };

    /// <Summary>
    /// Instruction to execute a subcircuit program
    /// </Summary>
    class apply_subcircuit : public executable {
        public: 
            /// <Summary>
            /// Name of subcircuit to run
            /// </Summary>
            std::string name;
            /// <Summary>
            /// Parameters to pass to subcircuit
            /// </Summary>
            std::vector<std::string> params;

            apply_subcircuit(string name) : name(name) {}
    };

    /// <Summary>
    /// Instruction to apply a gate to a register
    /// </Summary>
    class apply_gate : public executable {
        public:
            /// <Summary>
            /// Name of gate to apply
            /// </Summary>
            std::string name;
            /// <Summary>
            /// Name of registers to apply to (should be all the same)
            /// </Summary>
            std::vector<std::string> param_names;
            /// <Summary>
            /// Index of each register to apply to
            /// </Summary>
            std::vector<u64> param_indecies;

            apply_gate(string name): name(name){}

            virtual void invoke_rootprogram(runtime::environment& env) {
                igate* ptr = env.getGate(name);

                //Gate doesn't exist
                if(ptr == NULL){
                    stringstream sb;
                    sb << "No gate: \"";
                    sb << name;
                    sb << "\" exists";
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
                        sb << "Expecting gate to operate on register \"";
                        sb << name;
                        sb << "\" but got \"";
                        sb << (*it);
                        sb << "\"";
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                }

                if(env.hasQreg(name)){
                    env.getQreg(name).apply(*ptr, param_indecies);
                }else{
                    //Register doesn't exist
                    stringstream sb;
                    sb << "No quantum register: \"";
                    sb << name;
                    sb << "\" exists";
                    throw qasm::runtime::runtimeexception(sb.str());
                }

            };
    };

    /// <Summary>
    /// Instruction to print register to console
    /// </Summary>
    class print : public executable{
        public:
            /// <Summary>
            /// What variable to print to console
            /// </Summary>
            std::string reference;

            print(string ref): reference(ref){}

            virtual void invoke_rootprogram(runtime::environment& env) {
                std::string msg;
                std::cout << "print: ";
                if(env.hasCreg(reference)){
                    creg& r = env.getCreg(reference);
                    for (creg::const_reverse_iterator  i = r.rbegin(); i != r.rend(); i++)
                        std::cout << (*i == true ? "1" : "0");
                }else if(env.hasQreg(reference)){
                    std::cout << env.getQreg(reference).toString();
                }else{
                    //Throw exception
                    stringstream sb;
                    sb << "No register: \"";
                    sb << reference;
                    sb << "\" to print";
                    throw qasm::runtime::runtimeexception(sb.str());
                }
                std::cout << std::endl;
            };
    };

    /// <Summary>
    /// Instruction to measure a quantum variable into a classical one
    /// </Summary>
    class measurement : public executable {
        public: 
            /// <Summary>
            /// Quantum register name
            /// </Summary>
            std::string qreg;
            /// <Summary>
            /// Quantum register index
            /// </Summary>
            long qindex;
            /// <Summary>
            /// Classic register name
            /// </Summary>
            std::string creg;
            /// <Summary>
            /// Classic register index
            /// </Summary>
            long cindex;
            /// <Summary>
            /// Flag if whole register is to be measured or just the given indexes
            /// </Summary>
            bool measureWhole;

            measurement(string qr, long qi, string cr, long ci) : qreg(qr), qindex(qi), creg(cr), cindex(ci), measureWhole(false) {}
            measurement(string qr, string cr) : qreg(qr), qindex(-1), creg(cr), cindex(-1), measureWhole(true) {}
    
            virtual void invoke_rootprogram(runtime::environment& env) {
                if(!env.hasQreg(qreg) || !env.hasCreg(creg)){
                    stringstream sb;
                    sb << "Registers: \"";
                    sb << qreg << "\", \"" << creg << "\" don't correspond to a quantum and classical register pair";
                    throw qasm::runtime::runtimeexception(sb.str());
                }
                qlib::quantum::qreg& q = env.getQreg(qreg);
                std::vector<bool>& c = env.getCreg(creg);

                if(measureWhole){
                    for(size_t s = 0; s < q.size(); s++){
                        u8 value = q.measure(s);
                        bool isOne = value > 0;
                        if(s < c.size())
                            c[s] = isOne;
                    }
                }else {
                    
                    if(qindex < 0 || (ulong)qindex >= q.size()){
                        //Qbit index out of range
                        stringstream sb;
                        sb << "Quantum index: \"";
                        sb << qreg << qindex << "\" out of range";
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                    if(cindex < 0 || (ulong)cindex >= c.size()){
                        //Cbit index out of range
                        stringstream sb;
                        sb << "Classical index: \"";
                        sb << creg << cindex << "\" out of range";
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                    
                    u8 value = q.measure(qindex);
                    c[cindex] = value;
                }
            };
    };

    /// <Summary>
    /// Instruction to decare a variable
    /// </Summary>
    class declaration : public executable{
        public: 
            /// <Summary>
            /// Type of variable to declare
            /// </Summary>
            std::string type;
            /// <Summary>
            /// Name of variable to declare
            /// </Summary>
            std::string name;
            /// <Summary>
            /// Size of variable
            /// </Summary>
            u64 size;
        
            declaration(string t, string n, u64 s) : type(t), name(n), size(s) {}

            virtual void invoke_rootprogram(runtime::environment& env) {
                if(type == "qreg"){
                    env.setQreg(name, size);
                }else if(type == "creg"){
                    env.setCreg(name, size);
                }else{
                    stringstream sb;
                    sb << "Unknown type: \"";
                    sb << type;
                    sb << "\" can't define variable \"";
                    sb << name; 
                    sb << "\" of size ";
                    sb << size;
                    throw qasm::runtime::runtimeexception(sb.str());
                }
            };
    };

    /// <Summary>
    /// Instruction to import a file
    /// </Summary>
    class import : public executable {
        public:
            string filename;
            import(string file) : filename(file) {}
    };

    /// <Summary>
    /// Instruction to label a line
    /// </Summary>
    class label : public executable {
        public:
            string name;
            label(string name) : name(name) {}
    };

    /// <Summary>
    /// Instruction to reset a quantum or classical register
    /// </Summary>
    class reset : public executable {
        public:
            string name;
            string type;
            ulong index;
            bool useIndex;
            reset(string name, string type) : name(name), type(type), index(-1), useIndex(false) {}
            reset(string name, string type, ulong index) : name(name), type(type), index(index), useIndex(true) {}
            virtual void invoke_rootprogram(runtime::environment& env) {
                stringstream sb;
                if(type == "qreg"){
                    if(env.hasQreg(name)){
                        if(!useIndex){
                            qreg& q = env.getQreg(name);
                            env.setQreg(name, q.size());
                        }else{
                            qreg& q = env.getQreg(name);
                            q.zero(index);
                        }
                    }else{
                        sb << "Quantum register \"";
                        sb << name;
                        sb << "\" doesn't exist";
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                }else if(type == "creg"){
                    if(env.hasCreg(name)){
                        if(!useIndex){
                            creg& c = env.getCreg(name);
                            env.setCreg(name, c.size());
                        }else {
                            creg& c = env.getCreg(name);
                            c[index] = false;
                        }
                    }else{
                        sb << "Classic register \"";
                        sb << name;
                        sb << "\" doesn't exist";
                        throw qasm::runtime::runtimeexception(sb.str());
                    }
                }else{
                    sb << "Unknown type: \"";
                    sb << type;
                    sb << "\" to reset";
                    throw qasm::runtime::runtimeexception(sb.str());
                }
            }
    };
}
}

#endif