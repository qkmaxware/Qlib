#include <iostream>
#include <vector>

#include "qlib.h"

using namespace qlib::quantum;
using namespace qlib::math;
using namespace std;

void testOneQubitGate(std::string name, gates::igate* gate){
    //R bit qreg = 1|0> + 0|1>
    qreg r(1);
    cout << name << "|t> :: " << endl; 
    r.setState({1,0});
    cout << r.toString() << " -> ";
    r.apply(*gate, {0});
    cout << r.toString() << ", " << endl;
    r.setState({0,1});
    cout << r.toString() << " -> ";
    r.apply(*gate, {0});
    cout << r.toString() << ", " << endl; 
    r.setState({1,1});
    cout << r.toString() << " -> ";
    r.apply(*gate, {0});
    cout << r.toString() << endl;
}
void testControledQubitGate(std::string name, gates::igate* gate){

}
void testDoubleControledQubitGate(std::string name, gates::igate* gate){

}

int main(){
    
    //Test One-Qubit gates
    cout << "Testing 1-qubit gates" << endl;
    testOneQubitGate("H", &gates::H);
    testOneQubitGate("I", &gates::I);
    testOneQubitGate("X", &gates::X);
    testOneQubitGate("Y", &gates::Y);
    testOneQubitGate("Z", &gates::Z);
    testOneQubitGate("T", &gates::T);

    //Test two qubit gates
    cout << endl << "Testing 2-qubit gates" << endl;
    testControledQubitGate("CX", &gates::CX);
    testControledQubitGate("CY", &gates::CY);
    testControledQubitGate("CZ", &gates::CZ);

    //Test three qubit gates
    cout << endl << "Testing 3-qubit gates" << endl;
    testDoubleControledQubitGate("CCX", &gates::CCNOT);

    return 0;
}
