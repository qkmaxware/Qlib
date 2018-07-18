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
    //R bit qreg = 1|00> + 0|01> + 0|10> + 0|11>
    qreg r(2);
    cout << name << "|t> :: " << endl; 

    r.setState({1,0,0,0}); 
    cout << r.toString() << " -> "; r.apply(*gate, {0, 1}); cout << r.toString() << endl;
    r.setState({0,1,0,0}); 
    cout << r.toString() << " -> "; r.apply(*gate, {0, 1}); cout << r.toString() << endl;
    r.setState({0,0,1,0}); 
    cout << r.toString() << " -> "; r.apply(*gate, {0, 1}); cout << r.toString() << endl;
    r.setState({0,0,0,1}); 
    cout << r.toString() << " -> "; r.apply(*gate, {0, 1}); cout << r.toString() << endl;
    r.setState({1,1,1,1}); 
    cout << r.toString() << " -> "; r.apply(*gate, {0, 1}); cout << r.toString() << endl;
}


void testDoubleControledQubitGate(std::string name, gates::igate* gate){
    //R bit qreg = 1|000> + 0|001> + 0|010> + 0|011> + 0|100> + 0|101> + 0|110> + 0|111>
    qreg r(3);
    cout << name << "|t> :: " << endl; 
    
    for(int i = 0; i < 8; i++){
        r.setState({
            (i == 0 ? 1.0f : 0.0f),
            (i == 1 ? 1.0f : 0.0f),
            (i == 2 ? 1.0f : 0.0f),
            (i == 3 ? 1.0f : 0.0f),
            (i == 4 ? 1.0f : 0.0f),
            (i == 5 ? 1.0f : 0.0f),
            (i == 6 ? 1.0f : 0.0f),
            (i == 7 ? 1.0f : 0.0f),
        }); 
        cout << r.toString() << " -> "; r.apply(*gate, {0, 1, 2}); cout << r.toString() << endl;
    }
    r.setState({1,1,1,1,1,1,1,1}); 
    cout << r.toString() << " -> "; r.apply(*gate, {0, 1, 2}); cout << r.toString() << endl;
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
    cout << endl << "Testing 2-qubit gates ({0,1} control is 0 target is 1)" << endl;
    testControledQubitGate("CX", &gates::CX);
    testControledQubitGate("CY", &gates::CY);
    testControledQubitGate("CZ", &gates::CZ);

    //Test three qubit gates
    cout << endl << "Testing 3-qubit gates ({0, 1, 2} control are 0,1 target is 2)" << endl;
    testDoubleControledQubitGate("CCX", &gates::CCNOT);

    return 0;
}
