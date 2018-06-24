#Qlib - Core: 
##Quantum Computer Simulator

Qlib is a standalone C++ library, with no external dependancies, that simulates a "ideal" Quantum Computer.

A quantum computer is a computer where instead of operating on bits, it opertates on quantum bits 'qubits'. What is special about quantum bits is that they do not have a specific value. Instead they have some probability to be in any number of states. Classically, a bit can either be 0 or 1 whereas a qubit can be in some superposition of 0 and 1
```
Classic Bit:   x  = 0 or 1
Quantum Bit:  |x> = a * |0> + b * |1>
```
In the above description, a quantum bit is in state 0 with a probability of **a*a** and in state 1 with a probability of **b*b** where a and b are both [complex numbers](https://en.wikipedia.org/wiki/Complex_number). By applying unitary operators to this superposition of states, we can manipulate the probabilities to favor certain outcomes. We can determine the actual value of a quantum bit by measuring it. This interaction results in a "collapse" where the qubit is forced to choose if it is 0 or 1 and loses its superposition. Due to the probabilities involved, multiple identical qubits may produce different results when measured.

##Installation
```
git clone <repo>
```
Then simply reference the qlib.h file similarly to below which is a header that wrapps the whole library.
```cpp
#include "./qlib/src/core/qlib.h"
```
You can then use all the aspects of Qlib within your own applications. 

##Usage
A sample calculation is provided below in which we declar a quantum register of 2 qubits, apply 2 gates, and them measure the quantum system to collapse it into a specific state. Any of the gates in the table above can be accessed with the Alias attribute. 
```cpp
#include <iostream>
#include "./libs/qlib/src/core/qlib.h"

using namespace qlib::quantum;          //Quantum namespace to access qubit, qreg, ensemble
using namespace std;                    //Std namspace to access cout

int main(){
    //Create a quantum register of 2 qubits and print the superposition to terminal
    //All registers / bits are prepared in the |0..0> state
    qreg r(2);
    cout << r.toString() << endl;

    //Apply the Hadamard 'H' gate to qubit 0 of the register
    r.apply(gates::H, {0});
    //Apply the Controlled-Not 'CNOT' or 'CX' to control qubit 0 and target qubit 1
    r.apply(gates::CNOT, {0, 1});

    //Print out new superposition
    cout << r.toString() << endl;

    //Measure both qubits and collapse the state-vector. 
    //Will be either |00> or |11> with approximately 50% chance of either
    r.measure(0);
    r.measure(1);
    cout << r.toString() << endl;

    return 0;
}
```